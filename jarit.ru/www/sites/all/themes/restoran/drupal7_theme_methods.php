<?php

/* Drupal 7 methods definitons */

function d7_avto_8_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible art-postheader">' . t('You are here') . '</h2>';

    $output .= '<div class="breadcrumb art-postcontent">' . implode(' » ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Returns HTML for a button form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #button_type, #name, #value.
 *
 * @ingroup themeable
 */
function d7_avto_8_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'] . ' art-button';
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<span class="art-button-wrapper">'.
    '<span class="art-button-l"></span>'.
    '<span class="art-button-r"></span>'.
	'<input' . drupal_attributes($element['#attributes']) . ' />'.
	'</span>';
}

/**
 * Override or insert variables into the page template.
 */
function d7_avto_8_preprocess_page(&$vars) {
  $vars['tabs'] = menu_primary_local_tasks();
  $vars['tabs2'] = menu_secondary_local_tasks();
}

/**
 * Returns HTML for a single local task link.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing:
 *     - #link: A menu link array with 'title', 'href', and 'localized_options'
 *       keys.
 *     - #active: A boolean indicating whether the local task is active.
 *
 * @ingroup themeable
 */
function d7_avto_8_menu_local_task($variables) {
  $link = $variables['element']['#link'];
  $link_text = $link['title'];

  if (!empty($variables['element']['#active'])) {
    // Add text to indicate active tab for non-visual users.
    $active = '<span class="element-invisible">' . t('(active tab)') . '</span>';

    // If the link does not contain HTML already, check_plain() it now.
    // After we set 'html'=TRUE the link will not be sanitized by l().
    if (empty($link['localized_options']['html'])) {
      $link['title'] = check_plain($link['title']);
    }
    $link['localized_options']['html'] = TRUE;
    $link_text = t('!local-task-title!active', array('!local-task-title' => $link['title'], '!active' => $active));
  }

  //added art-class
  $link['localized_options']['attributes']['class'] = array('art-button');

  return '<li>' .
	  '<span class="art-button-wrapper">'.
	  '<span class="art-button-l"></span>'.
      '<span class="art-button-r"></span>'.
	  l($link_text, $link['href'], $link['localized_options']) .
	  "</span></li>\n";
}

/**
 * Returns HTML for a feed icon.
 *
 * @param $variables
 *   An associative array containing:
 *   - url: The url of the feed.
 *   - title: A descriptive title of the feed.
 */
function d7_avto_8_feed_icon($variables) {
  $text = t('Subscribe to @feed-title', array('@feed-title' => $variables['title']));
  return l(NULL, $variables['url'], array('html' => TRUE, 'attributes' => array('class' => array('feed-icon', 'art-rss-tag-icon'), 'title' => $text)));
}

/**
 * Returns HTML for a node preview for display during node creation and editing.
 *
 * @param $variables
 *   An associative array containing:
 *   - node: The node object which is being previewed.
 *
 * @ingroup themeable
 */
function d7_avto_8_node_preview($variables) {
  $node = $variables['node'];

  $output = '<div class="preview">';

  $preview_trimmed_version = FALSE;

  $elements = node_view(clone $node, 'teaser');
  $trimmed = drupal_render($elements);
  $elements = node_view($node, 'full');
  $full = drupal_render($elements);

  // Do we need to preview trimmed version of post as well as full version?
  if ($trimmed != $full) {
    drupal_set_message(t('The trimmed version of your post shows what your post looks like when promoted to the main page or when exported for syndication.<span class="no-js"> You can insert the delimiter "&lt;!--break--&gt;" (without the quotes) to fine-tune where your post gets split.</span>'));
	$preview_trimmed_version = t('Preview trimmed version');
	$output .= <<< EOT
<div class="art-post">
        <div class="art-post-tl"></div>
        <div class="art-post-tr"></div>
        <div class="art-post-bl"></div>
        <div class="art-post-br"></div>
        <div class="art-post-tc"></div>
        <div class="art-post-bc"></div>
        <div class="art-post-cl"></div>
        <div class="art-post-cr"></div>
        <div class="art-post-cc"></div>
        <div class="art-post-body">
    <div class="art-post-inner art-article">
    
<div class="art-postcontent">
    
      <h3>
	  $preview_trimmed_version
	  </h3>

    </div>
    <div class="cleared"></div>
    

    </div>
    
    		<div class="cleared"></div>
        </div>
    </div>
    
EOT;
	$output .= $trimmed;
    
	$preview_full_version = t('Preview full version');
	$output .= <<< EOT
<div class="art-post">
        <div class="art-post-tl"></div>
        <div class="art-post-tr"></div>
        <div class="art-post-bl"></div>
        <div class="art-post-br"></div>
        <div class="art-post-tc"></div>
        <div class="art-post-bc"></div>
        <div class="art-post-cl"></div>
        <div class="art-post-cr"></div>
        <div class="art-post-cc"></div>
        <div class="art-post-body">
    <div class="art-post-inner art-article">
    
<div class="art-postcontent">
    
      <h3>
	  $preview_full_version
	  </h3>

    </div>
    <div class="cleared"></div>
    

    </div>
    
    		<div class="cleared"></div>
        </div>
    </div>
    
EOT;

    $output .= $full;
  }
  else {
    $output .= $full;
  }
  $output .= "</div>\n";

  return $output;
}

/**
 * Return a Artisteer themed set of links.
 *
 * @param $content
 *   An object with node content.
 * @return
 *   A string containing an unordered list of links.
 */
function art_links_woker_D7($content) {
  $result = '';
  if (!isset($content['links'])) return $result;
  foreach (array_keys($content['links']) as $name) {
	$$name = & $content['links'][$name];
	if (isset($content['links'][$name]['#links'])) {
	  $links = $content['links'][$name]['#links'];
	  if (is_array($links)) {
		$output = get_links_html_output_D7($links);
		if (!empty($output)) {
			$result .= (empty($result)) ? $output : '&nbsp;|&nbsp;' . $output;
		}
	  }
    }
  }

$terms = get_terms_D7($content);
  if (!empty($terms)) {
  ob_start();?>
  <img class="art-metadata-icon" src="<?php echo get_full_path_to_theme(); ?>/images/posttagicon.png" width="18" height="18" alt="" /> <?php
  $result .= ($result == '') ? ob_get_clean() : '&nbsp;|&nbsp;' . ob_get_clean();
  $result .= '<div class="art-tags">' . render($terms) . '</div>';
  }
  

  return $result;  
}

function get_terms_D7($content) {
	$result = NULL;
	foreach (array_keys($content) as $name)	{
		$$name = & $content[$name];
	    $field_type = isset($content[$name]['#field_type']) ? $content[$name]['#field_type'] : NULL;
		if ($field_type == null || $field_type != "taxonomy_term_reference") continue;
	    $result = $content[$name];
	}
	return $result;
}

function get_links_html_output_D7($links) {
	$output = '';
	$num_links = count($links);
    $index = 0;

	foreach ($links as $key => $link) {
	  $class = array($key);

      // Add first, last and active classes to the list of links to help out themers.
      if ($index == 0) {
        $class[] = 'first';
      }
      if ($index == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      
	  $link_output = '';

      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $link_output = l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $link_output = '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }
		
if (strpos ($key, "comment") !== FALSE) {
		
		  if ($index > 0 && !empty($link_output) && !empty($output)) {
		  $output .= '&nbsp;|&nbsp;';
		}
		ob_start();?>
		<img class="art-metadata-icon" src="<?php echo get_full_path_to_theme(); ?>/images/postcommentsicon.png" width="18" height="18" alt="" /> <?php
		$output .= ob_get_clean();
		$output .= $link_output;
		$index++;
		continue;
		}
		
if ($index > 0 && !empty($link_output) && !empty($output)) {
          $output .= '&nbsp|&nbsp';
        }
        ob_start();?>
         <?php
        $output .= ob_get_clean();
        $output .= $link_output;
        $index++;
        

	}
	return $output;
}