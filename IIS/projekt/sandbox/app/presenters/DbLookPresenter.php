<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form;
use App\Presenters\DbAddPresenter;

class DbLookPresenter extends BasePresenter
{
	private $database;

    public function __construct(Nette\Database\Context $database)
    {
        $this->database = $database;
    }

	public function renderShowGate()
    {
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->gates = $this->database->table('Gate')->order('gate_id ASC');
    }

	public function renderShowFlight()
    {
		$list_of_other_tables = array();
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->flights = $this->database->table('Flight')->order('flight_id ASC');
		$this->template->internal_flights = $this->database->table('Internal_flight')->order('flight_id ASC');
		$this->template->international_flights = $this->database->table('International_flight')->order('flight_id ASC');
		
		
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
		/*
		#check if it is internal or international
		foreach($this->template->flights as $item)
		{
			$other_table_post = $this->database->table('Internal_flight')->get($item->flight_id);
			#internal flight
			if($other_table_post)
			{
				foreach($this->template->flights as $item2)
				{
					if($item2->flight_id == $other_table_post->flight_id)
					{
						array_push($item2, $other_table_post->representing_company);
						array_push($item2, $other_table_post->drink);
					}
				}
			}
			#international
			else
			{
				$other_table_post = $this->database->table('International_flight')->get($item->flight_id);
				foreach($this->template->flights as $item2)
				{
					if($item2->flight_id == $other_table_post->flight_id)
					{
						array_push($item2, $other_table_post->services);
						array_push($item2, $other_table_post->movies);
						array_push($item2, $other_table_post->food_list); 
					}
				}
			}
		}
		*/
		
    }
	public function renderShowPlane()
    {
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->planes = $this->database->table('Plane')->order('plane_id ASC');
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
	public function renderCheckRevision($plane_id)
    {
		$planes = $this->database->table('Plane')->order('plane_id ASC');
		$last_revision_string = NULL;
		$last_revision = NULL;
		foreach ($planes as $plane)
		{
			if($plane->plane_id == $plane_id)
			{
				$last_revision = $plane->last_revision;
				$last_revision_string = date("F jS, Y",strtotime($plane->last_revision)); #$plane->last_revision
			}
		}
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->last_revision = $last_revision;
		$this->template->actual_date = date("Y-m-d");
		#platnost revize
		
		$number_of_days = date_diff(date_create(date("Y-m-d")), date_create(date("Y-m-d", strtotime($last_revision_string))));
		if($number_of_days->format('%a days') > 730)
		{
			$this->template->is_valid = false;
		}
		else
		{
			$this->template->is_valid = true;
		}
    }
	public function renderShowEmployee()
    {
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->employees = $this->database->table('Airport_employee')->order('login ASC');
		$this->template->managerLogged = $this->getUser()->isInRole(Model\Role::MANAGER);
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
	
	public function renderShowTerminal()
    {
        if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->terminals = $this->database->table('Terminal')->order('terminal_id ASC');
    }
}
