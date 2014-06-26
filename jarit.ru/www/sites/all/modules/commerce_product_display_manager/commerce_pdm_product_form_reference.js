Drupal.behaviors.commerce_pdm_form_commerce_product_ui_product_form_alter = {
  attach: function(context, settings) {

    (function ($) {

      var COMMERCE_PDM_TYPE_MESSAGE_INFORMATION = 'commercePdmTypeMessageInfomration';
      var COMMERCE_PDM_TYPE_MESSAGE_ERROR = 'commercePdmTypeMessageError';

      // Declare all used elements as understandable variables.
      $table = $('table#commerce_pdm_product_referenced_by');
      $newTitleInput = $table.find('input#edit-quick-reference-add-new-reference-actions-commerce-pdm-product-display-title');
      $newTypeSelect = $table.find('select#edit-quick-reference-add-new-reference-actions-commerce-pdm-product-display-type');
      $existingNidInput = $table.find('input#edit-quick-reference-add-new-reference-actions-commerce-pdm-product-display-existing');
      $addButton = $table.find('a#edit-quick-reference-add-new-reference-actions-commerce-pdm-product-display-add');

      /**
       *
       */
      $addButton.bind('click', function(e) {
        if (!$table.find('div.add-new').hasClass('hidden')) {
          addNew();
        }
        else {
          addExisting();
        }

        return false;
      })

      /**
       *
       */
      $newTitleInput.bind('keypress', function(e) {
        if (e.keyCode == 13) {
          addNew();
          return false;
        }
      });

      /**
       *
       */
      $existingNidInput.bind('keypress', function(e) {
        // Accept only when the ENTER key has been presses and the autocomplete
        // list is hidden.
        if (e.keyCode == 13 && $(this).siblings('#autocomplete').length == 0) {
          addExisting();
          return false;
        }
      });

      /**
       *
       */
      function addExisting() {
        // Display throbber.
        $existingNidInput.css('background-position', '100% -16px');
        $.ajax({
          url: Drupal.settings.basePath + 'commerce_pdm/get_node_info/' + $existingNidInput.val(),
          dataTypeString: 'json',
          complete: function() {
            // Hide throbber.
            $existingNidInput.css('background-position', '100% 4px');
          },
          success: function(result) {
            if (result.success) {
              if ($('input.commerce-pdm-reference-nid[val="' + result.data.nid + '"]').length == 0) {
                addReferencedRow(result.data.title, result.data.nid, 0);
                displayMessage(Drupal.settings.commerce_pdm_product_form.added_existing, COMMERCE_PDM_TYPE_MESSAGE_INFORMATION);
              }
              else { // This product is already referenced by that display node.
                displayMessage(Drupal.settings.commerce_pdm_product_form.already_has_reference, COMMERCE_PDM_TYPE_MESSAGE_ERROR);
              }
            }
            else {
              if (result.message == 'node_full') {
                displayMessage(Drupal.settings.commerce_pdm_product_form.node_full, COMMERCE_PDM_TYPE_MESSAGE_ERROR);
              }
              else {
                displayMessage(Drupal.settings.commerce_pdm_product_form.node_not_found, COMMERCE_PDM_TYPE_MESSAGE_ERROR);
              }
            }
          }
        });
      }

      /**
       *
       */
      function addNew() {
        if ($newTitleInput.val().length > 0) {
          var displayNodeType = $newTypeSelect.val();
          var displatNodeTitle = $newTitleInput.val();

          displayMessage(Drupal.settings.commerce_pdm_product_form.added_new, COMMERCE_PDM_TYPE_MESSAGE_INFORMATION);
          addReferencedRow(displatNodeTitle, 'New (' + displayNodeType + ')', displayNodeType);
        }
        else {
          displayMessage(Drupal.settings.commerce_pdm_product_form.title_missing, COMMERCE_PDM_TYPE_MESSAGE_ERROR);
        }
      }

      /**
       *
       */
      function addReferencedRow(title, nid, newType) {
        var numRows =  $table .find('tr').length - 3;
        var $template = $table.find('tr.reference-row-template');
        var $new_row = $template.clone();

        $new_row.find('input.commerce-pdm-reference-attach').val(1);

        var $titleInput = $new_row.find('input#edit-quick-reference-commerce-pdm-product-reference-row-template-title');
        var $titleDiv = $new_row.find('div.form-item-quick-reference-commerce-pdm-product-reference-row-template-title');
        var $nidInput = $new_row.find('input#edit-quick-reference-commerce-pdm-product-reference-row-template-nid');
        var $nidDiv = $new_row.find('div.form-item-quick-reference-commerce-pdm-product-reference-row-template-nid');
        var $attachInput = $new_row.find('input.commerce-pdm-reference-attach');
        var $newTypeInput = $new_row.find('input.commerce-pdm-reference-new-type');
        var $newType = $new_row.find('input.commerce-pdm-reference-new-type');
        var $deleted = $new_row.find('input.commerce-pdm-reference-deleted');

        $new_row.find('a, input').attr('id', '');

        $titleInput.attr('id', 'edit-quick-reference-' + numRows + '-title');
        $titleInput.attr('name', 'quick_reference[' + numRows  + '][title]');
        $titleDiv.removeClass('form-item-quick-reference-commerce-pdm-product-reference-row-template-title');
        $titleDiv.addClass('form-item-quick-reference-' + numRows + '-title');

        $nidInput.attr('id', 'edit-quick-reference-' + numRows + '-nid');
        $nidInput.attr('name', 'quick_reference[' + numRows  + '][nid]');
        $nidInput.attr('val', nid);  // Because we need to check this later and value is not set???
        $nidDiv.removeClass('form-item-quick-reference-commerce-pdm-product-reference-row-template-nid');
        $nidDiv.addClass('form-item-quick-reference-' + numRows + '-nid');

        $newType.attr('name', 'quick_reference[' + numRows + '][new_type]');
        $deleted.attr('name', 'quick_reference[' + numRows + '][deleted]');
        $attachInput.attr('name', 'quick_reference[' + numRows + '][attach]');

        $new_row.removeClass('hidden').removeClass('reference-row-template');

        $titleInput.val(title);
        $nidInput.val(nid);
        $newTypeInput.val(newType);
        $new_row.find('input.commerce-pdm-new-reference').val(newType);

        $new_row.removeClass('odd').removeClass('even');
        $alternateRowClass = numRows % 2 ? 'even' : 'odd';
        $new_row.addClass($alternateRowClass);

        $template.before($new_row);
        $new_row.hide();
        $new_row.fadeIn(250);

        resetReferenceAddingRow();
      }

      /**
       *
       */
      function resetReferenceAddingRow() {
        $newTitleInput.val('');
        $existingNidInput.val('');
        toggleActionsVisible();
      }

      /**
       *
       */
      function displayMessage(msg, messageType) {
        $messageContainer = $('table#commerce_pdm_product_referenced_by div.commerce-pdm-product-form-reference-error-message');
        $messageContainer.css('color', (messageType == COMMERCE_PDM_TYPE_MESSAGE_INFORMATION) ? 'green' : 'red');
        $messageContainer.text(msg);
        $messageContainer.fadeIn(250).delay(1750).fadeOut(250);
      }

      /**
       *
       */
      $('table#commerce_pdm_product_referenced_by a.commerce-pdm-reference-delete').live('click', function() {
        $(this).parents('tr').addClass('deleted');
        $(this).siblings('input.commerce-pdm-reference-deleted').val(1);

        $(this).addClass('hidden');
        $(this).siblings('a.commerce-pdm-reference-undo-delete').removeClass('hidden');

        return false;
      });

      /**
       *
       */
      $('table#commerce_pdm_product_referenced_by a.commerce-pdm-reference-undo-delete').live('click', function() {
        $(this).parents('tr').removeClass('deleted');
        $(this).siblings('input.commerce-pdm-reference-deleted').val(0);

        $(this).addClass('hidden');
        $(this).siblings('a.commerce-pdm-reference-delete').removeClass('hidden');

        return false;
      });

      /**
       *
       */
      $('fieldset#edit-quick-reference div.commerce-pdm-product-form-reference-add-links a').bind('click', function(e) {
        var $add_new = $('fieldset#edit-quick-reference div.commerce-pdm-product-form-reference-add-actions div.add-new');
        var $existing = $('fieldset#edit-quick-reference div.commerce-pdm-product-form-reference-add-actions div.existing');
        if ($(this).attr('rel') == 'use_existing') {
          $add_new.addClass('hidden');
          $existing.removeClass('hidden');
        }
        else {
          $add_new.removeClass('hidden');
          $existing.addClass('hidden');
        }

        toggleActionsVisible();

        return false;
      });

      /**
       *
       */
      $table.find('a#edit-quick-reference-add-new-reference-actions-commerce-pdm-product-display-cancel').bind('click', function(e) {
        resetReferenceAddingRow();
        return false;
      });

      /**
       *
       */
      function toggleActionsVisible() {
        var $links = $('fieldset#edit-quick-reference div.commerce-pdm-product-form-reference-add-links');
        var $actions = $('fieldset#edit-quick-reference div.commerce-pdm-product-form-reference-add-actions');

        if ($links.hasClass('hidden')) {
          $links.removeClass('hidden');
          $actions.addClass('hidden');
        }
        else {
          $links.addClass('hidden');
          $actions.removeClass('hidden');
        }
      }

    })(jQuery);

  }
};
