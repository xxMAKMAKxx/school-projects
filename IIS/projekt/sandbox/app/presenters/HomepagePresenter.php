<?php

namespace App\Presenters;

use Nette;
use App\Model;


class HomepagePresenter extends BasePresenter
{
	private $database;
	
	public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	public function renderDefault()
	{
		$this->template->posts = $this->database->table('posts')
			->order('created_at DESC')
			->limit(5);
		if ($this->getUser()->isLoggedIn()) {
			$this->redirect('Menu:MainMenu');
		}
		$this->template->username = 'xkacma03';
	}
	protected function createComponentSignInForm()
	{
		$form = new Nette\Application\UI\Form;
		$form->addText('username', 'Uzivatelske jmeno:')
			->setRequired('Prosim vyplnte sve uzivatelske jmeno.');

		$form->addPassword('password', 'Heslo:')
			->setRequired('Prosim vyplnte sve heslo.');

		$form->addSubmit('send', 'Prihlasit');

		$form->onSuccess[] = array($this, 'signInFormSucceeded');
		return $form;
	}

	public function signInFormSucceeded($form)
	{
		$values = $form->values;

		try {
			$this->getUser()->login($values->username, $values->password);
			$this->getUser()->setExpiration('10 minutes', TRUE);
			$this->flashMessage("Byli ste uspesne prihlaseni.", 'success');
			sleep(1);
			$this->redirect('Menu:MainMenu');

		} catch (Nette\Security\AuthenticationException $e) {
			$form->addError('Nespravne prihlasovaci jmeno nebo heslo!');
		}
	}

}
