Drupal.behaviors.commerce_pdm_product_display_manager_form = {
  attach: function(context, settings) {

    (function ($) {

      if (typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag.commerce_pdm_product_display_manager == 'undefined') {
        return;
      }

      // Add drop listener for rows to set their css classes.
      Drupal.tableDrag.commerce_pdm_product_display_manager.onDrop = function () {

        $parent = $(this.oldRowElement).prevAll('tr.display-node:first');
        if ($parent.hasClass('orphans')) {
          $(this.oldRowElement).removeClass('product');
          $(this.oldRowElement).addClass('product-orphan');
        }
        else {
          $(this.oldRowElement).addClass('product');
          $(this.oldRowElement).removeClass('product-orphan');
        }

      }

    })(jQuery);

  }
};
