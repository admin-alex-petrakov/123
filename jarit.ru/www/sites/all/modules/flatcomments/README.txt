Flatcomments is a small module that allows you to force comments to always be
replies to the node, not other comments.

-- Example

 \- comment 1
 \- comment 2

Suppose user replies via the reply link on comment 1:

Without flatcomments:

 \- comment 1
    \- comment 3
 \- comment 2

With flatcomments enabled and flat list display for the content type:

 \- comment 1
 \- comment 2
 \- comment 3


-- Configuration

1. Enable the module.
2. Edit a content type.
3. Under the "Comment settings" section uncheck "Threading" checkbox.
4. (Optional) Hide reply links on comments by checking the
    "Do not show reply link on comments" checkbox at the bottom of the
    "Comment settings" section.

-- Developers

Current maintainer:
Alan Doucette (dragonwize)

Original creator:
Heine Deelstra (http://drupal.org/user/17943/contact)
