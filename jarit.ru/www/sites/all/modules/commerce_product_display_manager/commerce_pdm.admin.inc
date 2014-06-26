<?php

// Constant value for the title of the orphaned products table row.
define('COMMERCE_PDM_ORPHANS_ROW_TITLE', t('Products not assigned to any display node'));

/**
 * Display manager page.
 */
function commerce_pdm_product_display_manager_page() {

  $output = array();

  $output['filters'] = drupal_get_form('commerce_pdm_product_display_manager_filters');

  $output['admin'] = drupal_get_form('commerce_pdm_product_display_manager_form');

  return $output;
}

/**
 * Creates the display manager form.
 */
function commerce_pdm_product_display_manager_filters($form, &$form_state) {

  // Enable language column if translation module is enabled
  // or if we have any product display node with language.
  $multilanguage = (module_exists('translation') || db_query("SELECT COUNT(*) FROM {node} WHERE language <> :language", array(':language' => LANGUAGE_NONE))->fetchField());

  // Add filters to the form.
  $form['filter'] = product_manager_filter_form();
  $form['#submit'] = array('product_manager_filter_form_submit');

  return $form;
}

/**
 * Return form for products and product displays administration filters.
 */
function product_manager_filter_form() {

  $session = isset($_SESSION['products_overview_filter']) ? $_SESSION['products_overview_filter'] : array();
  $filters = product_manager_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'exposed_filters__node',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'term') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'term_load', $value);
      $value = $value->name;
    }
    elseif ($type == 'language') {
      $value = $value == LANGUAGE_NONE ? t('Language neutral') : module_invoke('locale', 'language_name', $value);
    }
    else {
      $value = $filters[$type]['options'][$value];
    }
    $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('and where %property is %value', $t_args));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('where %property is %value', $t_args));
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
    '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['status']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['status']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => count($session) ? t('Refine') : t('Filter'),
  );

  if (!empty($filters['language'])) {
    $form['filters']['status']['#suffix'] = '<small>' . t('Note that the product language will not be filtered, only the language of the product display node.') . '</small>';
  }

  if (count($session)) {
    $form['filters']['status']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['status']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * product_manager_filters.
 */
function product_manager_filters() {

  $filters = array();
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => array(
      '[any]' => t('any'),
      'status-1' => t('published'),
      'status-0' => t('not published'),
      'promote-1' => t('promoted'),
      'promote-0' => t('not promoted'),
      'sticky-1' => t('sticky'),
      'sticky-0' => t('not sticky'),
    ),
  );
  // Include translation states if we have this module enabled
  if (module_exists('translation')) {
    $filters['status']['options'] += array(
      'translate-0' => t('Up to date translation'),
      'translate-1' => t('Outdated translation'),
    );
  }

  $options = array();
  foreach (commerce_product_types() as $type => $product) {
    $options[$product['type']] = $product['name'];
  }
  $filters['product_type'] = array(
    'title' => t('product type'),
    'options' => array(
      '[any]' => t('any'),
    ) + $options,
  );

  $options = array();
  $product_display_types = _commerce_pdm_get_product_display_types(FALSE);
  foreach ($product_display_types as $node_type) {
    $options[$node_type['machine_name']] = $node_type['name'];
  }
  $filters['display_node_type'] = array(
    'title' => t('product display type'),
    'options' => array(
      '[any]' => t('any'),
    ) + $options,
  );

  // Language filter if there is a list of languages
  if ($languages = module_invoke('locale', 'language_list')) {
    $languages = array(LANGUAGE_NONE => t('Language neutral')) + $languages;
    $filters['language'] = array(
      'title' => t('language'),
      'options' => array(
        '[any]' => t('any'),
      ) + $languages,
    );
  }

  return $filters;
}

/**
 * Process result from product display administration filter form.
 */
function product_manager_filter_form_submit($form, &$form_state) {

  $filters = product_manager_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['products_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['products_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['products_overview_filter'] = array();
      break;
  }
}

/**
 * Creates the display manager form.
 *
 * @todo Typically, node entities are used as product displays. But there are no restrictions to
 *   turn any other entity type like terms, users, files, media,... into product displays too.
 *   PDM currently assumes all product displays are nodes.
 *   See: http://drupal.org/node/1376938
 */
function commerce_pdm_product_display_manager_form($form, &$form_state) {

  $form = array('#tree' => TRUE);
  $session = isset($_SESSION['products_overview_filter']) ? $_SESSION['products_overview_filter'] : array();

  $current_path = $_GET['q'];
  $product_display_types = _commerce_pdm_get_product_display_types(FALSE);

  // Skip the rest if no product displays where found.
  if (empty($product_display_types)) {
    return array('no_product_displays' => array(
      '#markup' => '<div style="margin-bottom:20px;">' . t('The store does not yet contain any product display nodes.') . '</div>',
    ));
  }
  $product_fields = _commerce_pdm_get_product_reference_fields();

  // Fetch the products & display nodes as a UNION query
  $union = FALSE;
  $base_query = NULL;
  foreach ($product_fields as $product_field) {
    $nodes_query = db_select('node', 'n');
    $nodes_query->leftJoin('field_data_' . $product_field, 'pr', 'pr.entity_id = n.nid');
    $nodes_query->leftJoin('commerce_product', 'cp', 'cp.product_id = pr.' . $product_field . '_product_id');
    $nodes_query->fields('cp', array('type', 'product_id', 'sku', 'title', 'language'));
    $nodes_query->fields('n', array('nid', 'title' => 'title', 'language', 'type', 'status', 'promote', 'sticky', 'translate'));
    $nodes_query->fields('pr', array('delta'));

    $display_types = array();
    foreach ($product_display_types as $product_display_type) {
      $display_types[] = $product_display_type['machine_name'];
    }
    $nodes_query->condition('n.type', $display_types, 'IN');

    if ($union) {
      $base_query->union($nodes_query, 'ALL');
    }
    else {
      $base_query = $nodes_query;
      $union = TRUE;
    }
  }

  // Add filter conditions to the union query
  $union = db_select($base_query, 'u');
  $union->fields('u');

  foreach ($session as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'status':
        // Note: no exploitable hole as $key/$value have already been checked when submitted
        list($key, $value) = explode('-', $value, 2);
        $union->condition('u.' . $key, $value);
        break;
      case 'display_node_type':
        $union->condition('u.n_type', $value);
        break;
      case 'product_type':
        $union->condition('u.type', $value);
        break;
      case 'language':
        $union->condition('u.n_language', $value);
        break;
    }
  }

  $nodes_query_result = $union->execute();

  $product_list = array();
  foreach ($nodes_query_result as $record) {
    // Create the Product Display when first time.
    if (!isset($product_list[$record->nid])) {
      $object = new stdClass();
      $object->nid = $record->nid;
      $object->title = $record->n_title;
      $object->type = $record->n_type;
      $object->product_type = $record->type;
      $object->language = $record->n_language;
      $object->children = array();
      $product_list[$record->nid] = $object;
    }

    if (!is_null($record->product_id)) {
      // Add the product to the list.
      $product_object = new stdClass();
      $product_object->product_id = $record->product_id;
      $product_object->sku = $record->sku;
      $product_object->type = $record->type;
      $product_object->title = $record->title;
      $product_object->language = $record->language;
      $product_list[$record->nid]->children[$record->delta] = $product_object;
    }
  }

  if (count($product_list) > 0) {
    // Add all display nodes and their referenced products to the form.
    foreach ($product_list as $node_obj) {
      $form[] = array(
        'title' => array(
          '#type' => 'item',
          '#markup' => $node_obj->title,
        ),
        'language' => array(
          '#type' => 'item',
          '#markup' => $node_obj->language,
        ),
        'edit_link' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => 'node/' . $node_obj->nid . '/edit',
              '#options' => array('query' => array('destination' => $current_path)),
        ),
        'delete_link' => array(
          '#type' => 'link',
          '#title' => t('Delete'),
          '#href' => 'node/' . $node_obj->nid . '/delete',
          '#options' => array('query' => array('destination' => $current_path)),
        ),
        'nid' => array(
          '#type' => 'hidden',
          '#value' => $node_obj->nid,
        ),
        '#type' => $node_obj->product_type,
      );

      if (!empty($node_obj->children)) {

        foreach ($node_obj->children as $delta => $related_product) {

          $form[] = array(
            'title' => array(
              '#type' => 'item',
              '#markup' => $related_product->title,
            ),
            'language' => array(
              '#type' => 'item',
              '#markup' => $related_product->language,
            ),
            'edit_link' => array(
              '#type' => 'link',
              '#title' => t('Edit'),
              '#href' => 'admin/commerce/products/' . $related_product->product_id . '/edit',
              '#options' => array('query' => array('destination' => $current_path)),
            ),
            'clone_link' => array(
              '#type' => 'link',
              '#title' => t('Clone'),
              '#href' => 'admin/commerce/products/clone/' . str_replace("_", "-", $related_product->type) . '/' . $related_product->product_id,
              '#options' => array('query' => array('pdid' => $node_obj->nid, 'destination' => $current_path)),
            ),
            'delete_link' => array(
              '#type' => 'link',
              '#title' => t('Delete'),
              '#href' => 'admin/commerce/products/' . $related_product->product_id . '/delete',
              '#options' => array('query' => array('destination' => $current_path)),
            ),
            'pid' => array(
              '#type' => 'hidden',
              '#default_value' => $related_product->product_id,
            ),
            'delta' => array(
              '#type' => 'weight',
              '#title' => t('Delta'),
              '#title_display' => 'invisible',
              '#default_value' => $delta,
            ),
          );

          $referenced_product_ids[] = $related_product->product_id;
        }
      }
    }

    // Load all products currently not referenced.
    $orphans_query = db_select('commerce_product', 'cp');
    $orphans_query->fields('cp', array('product_id', 'sku', 'title', 'language', 'type'));
    foreach ($product_fields as $field_name) {
      $orphans_query->leftJoin('field_data_' . $field_name, 'fd_' . $field_name, 'cp.product_id = fd_' . $field_name . '.' . $field_name . '_product_id');
      $orphans_query->condition('fd_' . $field_name . '.entity_id', NULL);
    }

    // Add only the product type filter here
    foreach ($session as $index => $filter) {
      list($key, $value) = $filter;
      switch ($key) {
        case 'product_type':
          $orphans_query->condition('cp.type', $value);
          break;
      }
    }

    $orphans_result = $orphans_query->execute();

    $form[] = array(
      'title' => array(
        '#type' => 'item',
        '#markup' => COMMERCE_PDM_ORPHANS_ROW_TITLE
      ),
      'nid' => array(
        '#type' => 'hidden',
        '#value' => 0,
      ),
      'language' => array(
        '#type' => 'item',
        '#markup' => '',
      ),
    );

    // Add all orphan products to the table.
    if ($orphans_result->rowCount() > 0) {
      foreach ($orphans_result as $orphan_key => $orphan_product) {
        $form[] = array(
          'title' => array(
            '#type' => 'item',
            '#markup' => $orphan_product->title,
          ),
          'language' => array(
            '#type' => 'item',
            '#markup' => $orphan_product->language,
          ),
          'edit_link' => array(
            '#type' => 'link',
            '#title' => t('Edit'),
            '#href' => 'admin/commerce/products/' . $orphan_product->product_id . '/edit',
            '#options' => array('query' => array('destination' => $current_path)),
          ),
          'delete_link' => array(
            '#type' => 'link',
            '#title' => t('Delete'),
            '#href' => 'admin/commerce/products/' . $orphan_product->product_id . '/delete',
            '#options' => array('query' => array('destination' => $current_path)),
          ),
          'pid' => array(
            '#type' => 'hidden',
            '#default_value' => $orphan_product->product_id,
          ),
          'orphan' => array(
            '#type' => 'hidden',
            '#default_value' => TRUE,
          ),
          'delta' => array(
            '#type' => 'weight',
            '#title' => t('Delta'),
            '#title_display' => 'invisible',
          ),
        );
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Adds JavaScript and CSS used by form.
  $form['#after_build'][] = '_commerce_pdm_product_display_manager_form_attach';

  // Turns it into a table.
  $form['#languages'] = module_invoke('locale', 'language_list');
  $form['#theme'] = 'commerce_pdm_product_display_manager_table';

  return $form;
}

/**
 * Creates a batch operation that will update all display nodes.
 */
function commerce_pdm_product_display_manager_form_validate($form, &$form_state) {

  $hierarchy = array();
  $current_display_node = NULL;
  foreach ($form_state['input'] as $input) {
    if (is_array($input)) {
      if (isset($input['nid'])) {
        if ($current_display_node) {
          $hierarchy[] = $current_display_node;
        }
        $current_display_node = array('nid' => $input['nid'], 'products' => array());
      }
      else {
        $current_display_node['products'][] = $input['pid'];
      }
    }
  }

  foreach ($hierarchy as $key => $display_node) {


    $node = node_load($display_node['nid']);
    if (!_commerce_pdm_get_number_reference_values_allowed($node, count($display_node['products']))) {
      form_set_error($key . '[nid]', t('Display node with title %note_title cannot contain the specified number of product references, field limit is set to %field_limit.',
        array('%note_title' => $node->title, '%field_limit' => _commerce_pdm_get_num_allowed_references($node))));
    }
  }
}

/**
 * Creates a batch operation that will update all display nodes.
 */
function commerce_pdm_product_display_manager_form_submit($form, &$form_state) {

  $hierarchy = array();
  $current_display_node = NULL;
  foreach ($form_state['input'] as $input) {
    if (is_array($input)) {
      if (isset($input['nid'])) {
        if ($current_display_node) {
          $hierarchy[] = $current_display_node;
        }
        $current_display_node = array('nid' => $input['nid'], 'products' => array());
      }
      else {
        $current_display_node['products'][] = $input['pid'];
      }
    }
  }

  // Create a skeleton for the update batch.
  $batch = array(
    'title' => t('Updating display nodes'),
    'operations' => array(),
    'finished' => '_commerce_pdm_display_nodes_updated',
    'file' => drupal_get_path('module', 'commerce_pdm') . '/commerce_pdm.admin.inc',
  );

  // Add operations.
  foreach ($hierarchy as $product_display_hierarchy) {
    $batch['operations'][] = array(
      '_commerce_pdm_update_display_node',
      array($product_display_hierarchy),
    );
  }

  // Initialize the update batch process.
  batch_set($batch);
}

/**
 * Displays the display manager form as a draggable table.
 */
function theme_commerce_pdm_product_display_manager_table($variables) {
  $form = &$variables['form'];
  $output = '';

  drupal_add_tabledrag('commerce_pdm_product_display_manager', 'order', 'sibling', 'delta');

  $output .= drupal_render($form['filter']);

  $links = array(
    array(
      'title' => t('Add product'),
      'href' => 'admin/commerce/products/add'
    ),
  );
  $output .= theme('links', array('links' => $links, 'attributes' => array('class' => array('action-links'))));

  $header = array (
    t('Product display title / product title'), t('Language'), t('Edit'), t('Delete'), t('Delta'),
  );

  $rows = array();
	foreach (element_children($form) as $key) {
    $form_element = &$form[$key];

    if (isset($form_element['title'])) {
      if (isset($form_element['nid'])) {  // It's a display node.
        $form_element['nid']['#attributes']['class'] = 'display-nid';

        $row = array();
        $row[] = drupal_render($form_element['title']) . drupal_render($form_element['nid']);
        $row[] = drupal_render($form_element['language']);
        $row[] = ($form_element['title']['#markup'] == COMMERCE_PDM_ORPHANS_ROW_TITLE) ? '' : drupal_render($form_element['edit_link']) .
          ' - ' . l(t('Add @type', array('@type' => $form_element['#type'])), 'admin/commerce/products/add/' . str_replace("_", "-", $form_element['#type']), array('query' => array('pdid' => $form_element['nid']['#value'], drupal_get_destination())));
        $row[] = ($form_element['title']['#markup'] == COMMERCE_PDM_ORPHANS_ROW_TITLE) ? '' : drupal_render($form_element['delete_link']);
        $row[] = '';

        $row_class = ($form_element['title']['#markup'] == COMMERCE_PDM_ORPHANS_ROW_TITLE) ? 'orphans' : '';
        $rows[] = array('data' => $row, 'class' => array('display-node', $row_class));
      }
      else if (!isset($form_element['orphan'])) {  // It's referenced product.
        $form_element['delta']['#attributes']['class'] = array('delta');

        $row = array();
        $row[] = theme('indentation', array('size' => 1)) . drupal_render($form_element['title']);
        $row[] = drupal_render($form_element['language']);
        $row[] = drupal_render($form_element['edit_link']) . ' - ' . drupal_render($form_element['clone_link']);
        $row[] = drupal_render($form_element['delete_link']);
        $row[] = drupal_render($form_element['delta']);

        $rows[] = array('data' => $row, 'class' => array('draggable', 'tabledrag-leaf', 'product'));
      }
      else {  // It's an orphan product.
        $form_element['delta']['#attributes']['class'] = array('delta');

        $row = array();
        $row[] = theme('indentation', array('size' => 1)) . drupal_render($form_element['title']);
        $row[] = drupal_render($form_element['language']);
        $row[] = drupal_render($form_element['edit_link']);
        $row[] = drupal_render($form_element['delete_link']);
        $row[] = drupal_render($form_element['delta']);

        $rows[] = array('data' => $row, 'class' => array('draggable', 'tabledrag-leaf', 'product-orphan'));
      }
		}
	}

  if (!empty($rows)) {
    $rows[0]['class'][] = 'first';
  }

  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'commerce_pdm_product_display_manager')));
  $output .= drupal_render_children($form);

  return $output;
}

/*
function commerce_pdm_form_commerce_product_ui_product_delete_form_alter(&$form, &$form_state, $form_id) {

}
function commerce_pdm_form_node_delete_confirm_alter(&$form, &$form_state, $form_id) {

}
*/

/**
 * Updates any given display node to reference to specified products.
 */
function _commerce_pdm_update_display_node($update_data, &$context) {
  $node = node_load($update_data['nid']);
  $context['message'] = t('Updating product display node %node_title', array('%node_title' => $node->title));

  $reference_field_name = _commerce_pdm_get_node_reference_field_name($node);
  $new_field_values = array();
  foreach($update_data['products'] as $product_id) {
    $new_field_values[] = array('product_id' => $product_id);
  }

  $original_field_values = field_get_items('node', $node, $reference_field_name);
  if ($new_field_values != $original_field_values) {
    $field_language = field_language('node', $node, $reference_field_name);
    if (!isset($original_field_values)) {
      $node->{$reference_field_name} = array($field_language => array());
    }
    $node->{$reference_field_name}[$field_language] = $new_field_values;
    node_save($node);
  }
}

/**
 * The display node's update batch is complete.
 */
function _commerce_pdm_display_nodes_updated($success, $results, $operations) {
  if ($success) {
    $message = t('Products and their display nodes has been updated successfully.');
  }
  else {
    $message = t('There was an error updating your products and their display nodes.');
  }

  drupal_set_message($message);
}

/**
 * Helper function to return all products that are referenced by a node.
 */
function _commerce_pdm_get_products_referenced($nid) {
  $product_fields = _commerce_pdm_get_product_reference_fields();
  $out = array();
  foreach ($product_fields as $field) {
    $result = db_query(
      'SELECT ' . $field . '_product_id FROM {field_data_' . $field . '} df WHERE df.entity_id = :eid ORDER BY df.delta',
      array(
        ':eid' => $nid,
      )
    );

    foreach ($result as $product_obj) {
      $out[] = $product_obj->{$field . '_product_id'};
    }
  }
  return array_values($out);
}

/**
 *  Add necessary css and JavaScript files to the display manager form.
 */
function _commerce_pdm_product_display_manager_form_attach($form_element) {
  drupal_add_css(drupal_get_path('module', 'commerce_pdm') . '/commerce_pdm_product_display_manager_form.css');
  drupal_add_js(drupal_get_path('module', 'commerce_pdm') . '/commerce_pdm_product_display_manager_form.js');
  return $form_element;
}
