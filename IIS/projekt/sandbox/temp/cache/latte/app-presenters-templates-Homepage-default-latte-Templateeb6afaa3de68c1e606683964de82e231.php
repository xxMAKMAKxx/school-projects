<?php
// source: /data/web/virtuals/107203/virtual/www/subdom/iis/sandbox/app/presenters/templates/Homepage/default.latte

class Templateeb6afaa3de68c1e606683964de82e231 extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('55eacc2e0e', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lbf511d65dd6_content')) { function _lbf511d65dd6_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?>    <h1>Letiste xkocou08</h1>
<?php if ($user->loggedIn) { ?>
        <li><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Post:show", array(2)), ENT_COMPAT) ?>
">Vstup do systemu</a></li>
<?php } else { ?>
        <h2>Je nutne se prihlasit pro vstup do systemu</h2>
		<center><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Sign:in"), ENT_COMPAT) ?>
">Přihlásit</a></center>
<?php } 
}}

//
// end of blocks
//

// template extending

$_l->extends = empty($_g->extended) && isset($_control) && $_control instanceof Nette\Application\UI\Presenter ? $_control->findLayoutTemplateFile() : NULL; $_g->extended = TRUE;

if ($_l->extends) { ob_start();}

// prolog Nette\Bridges\ApplicationLatte\UIMacros

// snippets support
if (empty($_l->extends) && !empty($_control->snippetMode)) {
	return Nette\Bridges\ApplicationLatte\UIRuntime::renderSnippets($_control, $_b, get_defined_vars());
}

//
// main template
//
if ($_l->extends) { ob_end_clean(); return $template->renderChildTemplate($_l->extends, get_defined_vars()); }
call_user_func(reset($_b->blocks['content']), $_b, get_defined_vars())  ?>

	<?php
}}