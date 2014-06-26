<?php

/**
 * Menu callback to clone a product.
 */
function commerce_pdm_product_ui_product_form_wrapper($product, $base_product) {

  // Override the base product values indicating this is a new one.
  $clone = clone($base_product);
  $clone->is_new = $product->is_new;
  $clone->created = $product->created;
  $clone->changed = $product->changed;
  $clone->status = $product->status;
  $clone->uid = $product->uid;
  $clone->product_id = $product->product_id;
  $clone->revision_id = $product->revision_id;

  module_load_include('inc', 'commerce_product_ui', 'includes/commerce_product_ui.products');

  return commerce_product_ui_product_form_wrapper($clone);

}
