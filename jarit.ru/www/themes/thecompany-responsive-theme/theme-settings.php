<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function thecompany_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['prof_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Professional Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['prof_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs','thecompany'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['prof_settings']['top_social_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links in header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['prof_settings']['top_social_link']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show social icons (Facebook, Twitter and RSS) in header'),
    '#default_value' => theme_get_setting('social_links', 'thecompany'),
    '#description'   => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['prof_settings']['top_social_link']['twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter profile url'),
    '#default_value' => theme_get_setting('twitter_url', 'thecompany'),
    '#description' => t("Enter your Twitter profile url."),
  );
  $form['prof_settings']['top_social_link']['facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook profile url'),
    '#default_value' => theme_get_setting('facebook_url', 'thecompany'),
    '#description' => t("Enter your Facebook profile url."),
  );


  $form['prof_settings']['top_social_link']['gplus_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Gplus profile url'),
    '#default_value' => theme_get_setting('gplus_url', 'thecompany'),
    '#description' => t("Enter your Gplus profile url."),
  );
  $form['prof_settings']['top_social_link']['linkedin_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Linkedin profile url'),
    '#default_value' => theme_get_setting('linkedin_url', 'thecompany'),
    '#description' => t("Enter your Linkedin profile url."),
  );
  $form['prof_settings']['top_social_link']['dribbble_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Dribbble profile url'),
    '#default_value' => theme_get_setting('dribbble_url', 'thecompany'),
    '#description' => t("Enter your Dribbble profile url."),
  );
  $form['prof_settings']['top_social_link']['vimeo_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo profile url'),
    '#default_value' => theme_get_setting('vimeo_url', 'thecompany'),
    '#description' => t("Enter your Vimeo profile url."),
  );
  $form['prof_settings']['top_social_link']['youtube_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Youtube profile url'),
    '#default_value' => theme_get_setting('youtube', 'thecompany'),
    '#description' => t("Enter your Youtube profile url."),
  );

}


