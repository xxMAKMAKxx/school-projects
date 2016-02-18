<?php

namespace App\Presenters;

use Nette;
use App\Model;


class MenuPresenter extends BasePresenter
{
	private $database;
	
	public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	public function renderMainMenu()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		if($this->getUser()->isInRole(Model\Role::MANAGER)) {
			$this->template->EmpType = 3;
		}
		else if($this->getUser()->isInRole(Model\Role::SUPERVISOR)) {
			$this->template->EmpType = 2;
		}
		else
		{
			$this->template->EmpType = 1;
		}
	}

}
