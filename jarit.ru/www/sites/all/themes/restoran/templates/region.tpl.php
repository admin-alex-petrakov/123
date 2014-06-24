<?php if ($content): ?>
  <div class="<?php print $classes; ?>">
    <?php switch ($region) {
			case "navigation":
				print art_menu_worker($content, true, 'art-hmenu');
				break;
			case "vnavigation_left":
			case "vnavigation_right":
foreach (array_keys($variables['elements']) as $name) {
					$element = &$variables['elements'][$name];
					if (is_array($element)
					&& isset($element['#block']) && isset($element['#block']->subject)
				&& isset($element['#children']) && is_string($element['#children']) && !empty($element['#children'])) {
						$block_subject = $element['#block']->subject;
						$block_content = $element['#children'];
						art_vmenu_output($block_subject, $block_content);	}
					}
				
				break;
			default:
				print $content;
				break;
		}?>
  </div>
<?php endif; ?>