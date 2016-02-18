<?php
// source: /data/web/virtuals/107203/virtual/www/subdom/iis/sandbox/app/presenters/templates/Post/edit.latte

class Template7289d0ba733c095c22f38dc2373456da extends Latte\Template {
function render() {
foreach ($this->params as $__k => $__v) $$__k = $__v; unset($__k, $__v);
// prolog Latte\Macros\CoreMacros
list($_b, $_g, $_l) = $template->initialize('4f27ec927b', 'html')
;
// prolog Latte\Macros\BlockMacros
//
// block content
//
if (!function_exists($_b->blocks['content'][] = '_lbe2f17ccd02_content')) { function _lbe2f17ccd02_content($_b, $_args) { foreach ($_args as $__k => $__v) $$__k = $__v
?><h2>Uprava prispevku</h2>

<?php $_l->tmp = $_control->getComponent("postForm"); if ($_l->tmp instanceof Nette\Application\UI\IRenderable) $_l->tmp->redrawControl(NULL, FALSE); $_l->tmp->render() ?>


<p><a href="<?php echo Latte\Runtime\Filters::escapeHtml($_control->link("Post:show", array($post->id)), ENT_COMPAT) ?>
">Zpet na prispevek</a></p><?php
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
call_user_func(reset($_b->blocks['content']), $_b, get_defined_vars()) ; 
}}