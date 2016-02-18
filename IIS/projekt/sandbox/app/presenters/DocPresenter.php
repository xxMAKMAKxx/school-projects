<?php

namespace App\Presenters;

use Nette;
use App\Model;


class DocPresenter extends BasePresenter
{
	private $database;
	
	public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	public function renderDefault()
	{

	}
}
