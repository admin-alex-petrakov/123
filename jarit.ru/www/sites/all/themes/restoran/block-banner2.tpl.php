<div id="<?php print $block->region .'-'. $block->module .'-'. $block->delta; ?>">
  <?php if (!empty($block->subject)): ?>
    <h2><?php print $block->subject ?></h2>
  <?php endif;?>	
  <div class="content">
    <?php print $block->content ?>
  </div>
</div>