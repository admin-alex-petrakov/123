<?php

/* Drupal 5 methods definitons */

function d7_avto_8_regions() {
  return array(
'sidebar_left' => t('Left sidebar'),
  'vnavigation_left' => ('Left vertical menu'),
  
  'content'  => t('Content'),
	'navigation'  => t('Menu'),
	'banner1'  => t('Banner 1'),
	'banner2'  => t('Banner 2'),
	'banner3'  => t('Banner 3'),
	'banner4'  => t('Banner 4'),
	'banner5'  => t('Banner 5'),
	'banner6'  => t('Banner 6'),
	'user1'  => t('User 1'),
	'user2'  => t('User 2'),
	'user3'  => t('User 3'),
	'user4'  => t('User 4'),
	'extra1'  => t('Extra 1'),
	'extra2'  => t('Extra 2'),
	'copyright'  => t('Copyright'),
	'top1' => t('Top 1'),
    'top2' => t('Top 2'),
    'top3' => t('Top 3'),
    'bottom1' => t('Bottom 1'),
    'bottom2' => t('Bottom 2'),
    'bottom3' => t('Bottom 3'));
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {
  if ($hook != 'page') {
	return array();
  }
  $vars['tabs'] = menu_primary_local_tasks();
  $vars['tabs2'] = menu_secondary_local_tasks();
  
  // Make $front_page variable available
  $vars['front_page'] = url();

  drupal_add_js(path_to_theme() .'/script.js', 'theme');
  $vars['scripts'] = drupal_get_js();
  return $vars;  
}

/**
 * Generate the HTML representing a given menu item ID as a tab.
 *
 * @param $mid
 *   The menu ID to render.
 * @param $active
 *   Whether this tab or a subtab is the active menu item.
 * @param $primary
 *   Whether this tab is a primary tab or a subtab.
 *
 * @ingroup themeable
 */
function d7_avto_8_menu_local_task($mid, $active, $primary) {
  $active_class = "";
  if ($active) {
    $active_class .= "active ";
  }
  $link = menu_item_link($mid, FALSE);
  $output = '<span class="'.$active_class.'art-button-wrapper">'.
    '<span class="art-button-l"></span>'.
    '<span class="art-button-r"></span>'.
    '<a href="?q='.$link['href'].'" class="'.$active_class.'art-button">'.$link['title'].'</a></span>';
  return '<li>'.$output.'</li>';
}

/**
 * Return code that emits an feed icon.
 *
 * @param $url
 *   The url of the feed.
 */
function d7_avto_8_feed_icon($url) {
  return '<a href="'. check_url($url) .'" class="art-rss-tag-icon" title="' . t('Syndicate content') . '"></a>';
}

/**
 * Allow themable wrapping of all comments.
 */
function d7_avto_8_comment_wrapper($content, $type = null) {
  static $node_type;
  if (isset($type)) $node_type = $type;
  
  ob_start();?>
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
  
  <?php $result .= ob_get_clean();

  if ($content && $node_type != 'forum') {
    $result .= '<h2 class="art-postheader comments">' . t('Comments') . '</h2>';
  }
  
  ob_start();?>
<div class="art-postcontent">
  
  <?php $result .= ob_get_clean();
  
  $result .= $content;
  ob_start();?>

  </div>
  <div class="cleared"></div>
  
  <?php $result .= ob_get_clean();
  
  ob_start();?>

  </div>
  
  		<div class="cleared"></div>
      </div>
  </div>
  
  <?php $result .= ob_get_clean();
  
  return $result;
}

/**
 * Allow themable wrapping of all breadcrumbs.
 */
function d7_avto_8_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb art-postcontent">'. implode(' | ', $breadcrumb) .'</div>';
  }
}

function d7_avto_8_service_links_node_format($links) {
  return '<div class="service-links"><div class="service-label">'. t('Bookmark/Search this post with: ') .'</div>'. art_links_woker($links) .'</div>';
}

/**
 * Theme a form button.
 *
 * @ingroup themeable
 */
function d7_avto_8_button($element) {
  // Make sure not to overwrite classes.
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-'.$element['#button_type'].' '.$element['#attributes']['class'].' art-button';
  }
  else {
    $element['#attributes']['class'] = 'form-'.$element['#button_type'].' art-button';
  }
   	
  return '<span class="art-button-wrapper">'.
    '<span class="art-button-l"></span>'.
    '<span class="art-button-r"></span>'.
    '<input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name']
         .'" ')  .'id="'. $element['#id'].'" value="'. check_plain($element['#value']) .'" '. drupal_attributes($element['#attributes']).'/>'.
	'</span>';
}

/**
 * Image assist module support.
 * Added Artisteer styles in IE
*/
function d7_avto_8_img_assist_page($content, $attributes = NULL) {
  $title = drupal_get_title();
  $output = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">'."\n";
  $output .= '<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">'."\n";
  $output .= "<head>\n";
  $output .= '<title>'. $title ."</title>\n";
  
  // Note on CSS files from Benjamin Shell:
  // Stylesheets are a problem with image assist. Image assist works great as a
  // TinyMCE plugin, so I want it to LOOK like a TinyMCE plugin. However, it's
  // not always a TinyMCE plugin, so then it should like a themed Drupal page.
  // Advanced users will be able to customize everything, even TinyMCE, so I'm
  // more concerned about everyone else. TinyMCE looks great out-of-the-box so I
  // want image assist to look great as well. My solution to this problem is as
  // follows:
  // If this image assist window was loaded from TinyMCE, then include the
  // TinyMCE popups_css file (configurable with the initialization string on the
  // page that loaded TinyMCE). Otherwise, load drupal.css and the theme's
  // styles. This still leaves out sites that allow users to use the TinyMCE
  // plugin AND the Add Image link (visibility of this link is now a setting).
  // However, on my site I turned off the text link since I use TinyMCE. I think
  // it would confuse users to have an Add Images link AND a button on the
  // TinyMCE toolbar.
  // 
  // Note that in both cases the img_assist.css file is loaded last. This
  // provides a way to make style changes to img_assist independently of how it
  // was loaded.
  $output .= drupal_get_html_head();
  $output .= drupal_get_js();
  $output .= "\n<script type=\"text/javascript\"><!-- \n";
  $output .= "  if (parent.tinyMCE && parent.tinyMCEPopup && parent.tinyMCEPopup.getParam('popups_css')) {\n";
  $output .= "    document.write('<link href=\"' + parent.tinyMCEPopup.getParam('popups_css') + '\" rel=\"stylesheet\" type=\"text/css\">');\n";
  $output .= "  } else {\n";
  foreach (drupal_add_css() as $media => $type) {
    $paths = array_merge($type['module'], $type['theme']);
    foreach (array_keys($paths) as $path) {
      // Don't import img_assist.css twice.
      if (!strstr($path, 'img_assist.css')) {
        $output .= "  document.write('<style type=\"text/css\" media=\"{$media}\">@import \"". base_path() . $path ."\";<\/style>');\n";
      }
    }
  }
  $output .= "  }\n";
  $output .= "--></script>\n";
  // Ensure that img_assist.js is imported last.
  $path = drupal_get_path('module', 'img_assist') .'/img_assist_popup.css';
  $output .= "<style type=\"text/css\" media=\"all\">@import \"". base_path() . $path ."\";</style>\n";
  
  $output .= '<link rel="stylesheet" href="'.get_full_path_to_theme().'/style.css" type="text/css" />'."\n";
  $output .= '<!--[if IE 6]><link rel="stylesheet" href="'.get_full_path_to_theme().'/style.ie6.css" type="text/css" /><![endif]-->'."\n";
  $output .= '<!--[if IE 7]><link rel="stylesheet" href="'.get_full_path_to_theme().'/style.ie7.css" type="text/css" /><![endif]-->'."\n";

  $output .= "</head>\n";
  $output .= '<body'. drupal_attributes($attributes) .">\n";
  
  $output .= theme_status_messages();
  
  $output .= "\n";
  $output .= $content;
  $output .= "\n";
  $output .= '</body>';
  $output .= '</html>';
  return $output;
}