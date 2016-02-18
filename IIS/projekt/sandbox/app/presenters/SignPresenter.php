<?php

namespace App\Presenters;

use Nette;
use App\Forms\SignFormFactory;


class SignPresenter extends BasePresenter
{
	/** @var SignFormFactory @inject */
	public $factory;


	/**
	 * Sign-in form factory.
	 * @return Nette\Application\UI\Form
	 */
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
			$this->redirect('Homepage:');

		} catch (Nette\Security\AuthenticationException $e) {
			$form->addError('Nesprávné přihlašovací jméno nebo heslo.');
		}
	}

	public function actionOut()
	{
		$this->getUser()->logout();
		$this->flashMessage('Odhlášení bylo úspěšné.');
		$this->redirect('Homepage:');
	}
	
	

}
