<?php

namespace App\Presenters;

use Nette;
use App\Model;
use Nette\Application\UI\Form;
use Tracy\Debugger;

class DbAddPresenter extends BasePresenter
{
	private $database;
	private $typeOfFlightGlobal = -1;
	private $ticket_id;
	private $editFlightId;
	private $oldFlightPlaneId;
	private $flight_id;

	public function __construct(Nette\Database\Context $database)
	{
		$this->database = $database;
	}

	#check if date is in correct format
	public function checkDate($value)
	{
		$pattern = '/^[1-2][0-9][0-9][0-9]-(1[1-2]|0[0-9])-[0-3][0-9]$/';
		$pattern2 = '/^[1-2][0-9][0-9][0-9]-(1[1-2]|0[0-9])-[0-3][0-9]\s(1[0-9]|2[0-4]|0[0-9]):[0-6][0-9]:[0-6][0-9]$/';
		if(preg_match($pattern, $value))
			return true;
		else if(preg_match($pattern2, $value))
			return true;
		else
			return false;
	}
	#check if Timestamp is in correct format
	public function checkTimestamp($value)
	{
		$pattern = '/^[1-2][0-9][0-9][0-9]-(1[0-2]|0[1-9])-[0-3][0-9]\s(1[0-9]|2[0-4]|0[0-9]):[0-6][0-9]:[0-6][0-9]$/';
		if(preg_match($pattern, $value))
			return true;
		else
			return false;
	}
	#sekce s gaty
	public function renderAddGate()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}

		if(!$this->getUser()->isInRole(Model\Role::MANAGER)) {
			$this->redirectDeniedAction('Menu:MainMenu');
		}
	}

	protected function createComponentAddGateForm()
	{


		#get list of terminals
		$terminals = array();
		$terminals_db = $this->database->table('Terminal');
		foreach ( $terminals_db as $item )
		{
			array_push($terminals, $item->terminal_id);
		}

		$gateType = array(
				'EXIT with BUS' => 'EXIT with BUS',
				'EXIT standard' => 'EXIT standard',
		);
		$form = new Form;

		$form->addText('capacity', 'Kapacita:');

		$form->addSelect('gate_type', 'Typ gatu:')->setItems($gateType, FALSE);



		$form->addSelect('terminal_id', 'ID Terminalu:')->setItems($terminals, FALSE);

		$form->addSubmit('send', 'Pridat gate');
		$form->onSuccess[] = array($this, 'addGateFormSucceeded');
		return $form;
	}

	public function addGateFormSucceeded($form, $values)
	{
		if(!is_numeric($values->capacity) && $values->capacity)
		{
			$this->flashMessage('Chyba! Kapacita neni cislo!', 'error');
		}
		else
		{

			$this->database->table('Gate')->insert(array(
					'capacity' => $values->capacity,
					'gate_type' => $values->gate_type,
					'terminal_id' => $values->terminal_id,
			));

			$this->flashMessage('Gate uspesne vlozen', 'success');
			$this->redirect('this');
		}
	}

	public function renderAddPlane()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		if(!$this->getUser()->isInRole(Model\Role::MANAGER)) {
			$this->redirectDeniedAction('Menu:MainMenu');
		}
	}

	protected function createComponentAddPlaneForm()
	{


		#get list of terminals
		$planeTypes = array();
		$planeTypes_db = $this->database->table('Plane_type');
		foreach ( $planeTypes_db as $item )
		{
			array_push($planeTypes, $item->plane_type);
		}

		$form = new Form;

		$form->addText('plane_id', 'ID letadla:')->setRequired(); #->setOption('description', 'Toto ��slo z�stane skryt�');
		$form->addText('manufacture_date', 'Datum vyroby:')->setOption('description', '(RRRR-MM-DD format)');
		$form->addText('last_revision', 'Datum posledni revize:')->setOption('description', '(RRRR-MM-DD format)')->setRequired();
		$form->addText('crew_number', 'Pocet clenu posadky:');


		$form->addSelect('plane_type', 'Typ letadla:')->setItems($planeTypes, FALSE);

		$form->addSubmit('send', 'Pridat letadlo');
		$form->onSuccess[] = array($this, 'addPlaneFormSucceeded');
		return $form;
	}

	public function addPlaneFormSucceeded($form, $values)
	{
		if(!is_numeric($values->crew_number) && $values->crew_number)
		{
			$this->flashMessage('Chyba! Pocet clenu posadky neni cislo!', 'error');
		}
		else if(!$this->checkDate($values->manufacture_date) && $values->manufacture_date)
		{
			$this->flashMessage('Chyba! Spatny format data vyroby!', 'error');
		}
		else if(!$this->checkDate($values->last_revision) && $values->last_revision)
		{
			$this->flashMessage('Chyba! Spatny format data posledni revize!', 'error');
		}
		else if($values->crew_number < 1 && $values->crew_number)
		{
			$this->flashMessage('Chyba! Pocet clenu posadky musi byt kladny!', 'error');
		}
		else
		{
			#check that ID of the plane is unique
			$planeId = $this->getParameter('planeId');
			if ($planeId)
			{
				$post = $this->database->table('Plane')->get($planeId);
				$post->update(array(
						'plane_id' => $values->plane_id,
						'manufacture_date' => $values->manufacture_date,
						'last_revision' => $values->last_revision,
						'crew_number' => $values->crew_number,
						'plane_type' => $values->plane_type,
				));
				$this->flashMessage('Letadlo uspesne aktualizovano', 'success');
				$this->redirect('this');
			}
			else
			{
				if($this->checkPlaneID($values->plane_id))
				{
					$this->database->table('Plane')->insert(array(
							'plane_id' => $values->plane_id,
							'manufacture_date' => $values->manufacture_date,
							'last_revision' => $values->last_revision,
							'crew_number' => $values->crew_number,
							'plane_type' => $values->plane_type,
					));
					$seat = $this->database->table('Seat');
					for($i = 1; $i <= 10; $i++) {
						$seat->insert(array(
								'seat_number' => $i,
								'class' => $i < 5 ? 1 : 2,
								'sector' => $i%2 == 0 ? 'A' : 'B',
								'plane_id' => $values->plane_id
						));
					}
					$this->flashMessage('Letadlo uspesne vlozeno', 'success');
					$this->redirect('this');
				}
				else
				{
					$this->flashMessage('Chyba! Letadlo s timto ID jiz existuje!', 'error');
				}
			}
		}
	}

	private function checkPlaneID($planeId)
	{
		$planeID_db = $this->database->table('Plane');
		foreach ( $planeID_db as $item )
		{
			if(strcmp($planeId, $item) == 0)
			{
				return false;
			}
		}
		return true;
	}

	#flight section - 0 = vnitrnostatni, 1 = mezinarodni
	public function renderAddFlight($typeOfFlight)
	{
		$user = $this->getUser();
		if (!$user->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		if(!$user->isInRole(Model\Role::MANAGER)) {
			$this->redirectDeniedAction('Menu:MainMenu');
		}
		$this->template->typeOfFlight = $typeOfFlight;
	}

	protected function createComponentAddFlightForm()
	{
		#get type of flight from render function
		$typeOfFlight = $this->getParameter('typeOfFlight');
		#get plane IDs
		$planeIDs = array();
		$planeIDs_db = $this->database->table('Plane');
		foreach ( $planeIDs_db as $item )
		{
			array_push($planeIDs, $item->plane_id);
		}
		#get gate IDs
		$gateIDs = array();
		$gateIDs_db = $this->database->table('Gate');
		foreach ( $gateIDs_db as $item )
		{
			array_push($gateIDs, $item->gate_id);
		}
		#create form
		$form = new Form;
		$form->addText('departure', 'Cas odletu:')->setOption('description', '(RRRR-MM-DD HH:MM:SS format)');
		$form->addText('arrival', 'Cas priletu:')->setOption('description', '(RRRR-MM-DD HH:MM:SS format)');
		$form->addText('from', 'Odlet z:');
		$form->addText('to', 'Prilet do:');
		$form->addText('company', 'Spolecnost:');
		$form->addSelect('plane_id', 'ID letdla:')->setItems($planeIDs, FALSE);
		$form->addSelect('gate_id', 'ID gatu:')->setItems($gateIDs, FALSE);
		#vnitrnostatni

		if($typeOfFlight == 0 && $this->typeOfFlightGlobal != 1)
		{
			$form->addText('representing_company', 'Zastupuje:')->setOption('description', '(Zastupujici spolecnost)');
			$form->addTextArea('drink', 'Napoje nabidnute cestujicim:')->setOption('description', '(Retezec kdy polozky jsou oddelen carkou.)');
		}
		#mezistatni
		else
		{
			$form->addTextArea('services', 'Sluzby nabidnute cestujicim:')->setOption('description', '(Retezec kdy polozky jsou oddelen carkou.)');
			$form->addTextArea('movies', 'Filmy nabidnute cestujicim:')->setOption('description', '(Retezec kdy polozky jsou oddelen carkou.)');
			$form->addTextArea('food_list', 'Jidla nabidnute cestujicim:')->setOption('description', '(Retezec kdy polozky jsou oddelen carkou.)');
		}
		#odeslat
		if(isset($this->editFlightId)) {
			$form->addSubmit('send', 'Upravit let');
		}
		else{
			$form->addSubmit('send', 'Pridat let');
		}
		$form->onSuccess[] = array($this, 'addFlightFormSucceeded');
		return $form;
	}

	public function addFlightFormSucceeded($form, $values)
	{
		$typeOfFlight = $this->getParameter('typeOfFlight');
		$plane_type = $this->database->table('Plane')->where('plane_id', $values->plane_id)->fetch()->plane_type;
		if (preg_match('/[0-9]+/', $values->from) && $values->from)
		{
			$this->flashMessage('Chyba! Spatny format mista odletu!', 'error');
		}
		else if (preg_match('/[0-9]+/', $values->to) && $values->to)
		{
			$this->flashMessage('Chyba! Spatny format mista priletu!', 'error');
		}
		else if(!$this->checkTimestamp($values->arrival) && $values->arrival)
		{
			$this->flashMessage('Chyba! Spatny format casu priletu!', 'error');
		}
		else if(!$this->checkTimestamp($values->departure) && $values->departure)
		{
			$this->flashMessage('Chyba! Spatny format casu odletu!', 'error');
		}
		elseif(!$this->canPlaneFlyFromGate($plane_type, $values->gate_id, $canFlyFrom)) {
			$canFlyFromString = implode(",", $canFlyFrom);
			$this->flashMessage('Chyba! Dane lietadlo typu '. $plane_type .' nemuze odletat z gatu '. $values->gate_id.'!'."\n".
					"Dane lietadlo moze odlietat z gatu: $canFlyFromString", 'error');
		}
		else
		{
			#odeslani do tabulky letu
			$flightId = $this->getParameter('flightId');
			if($flightId)
			{
				$this->database->table('Flight')->get($flightId)->update(array(
					'departure' => $values->departure,
					'arrival' => $values->arrival,
					'from' => $values->from,
					'to' => $values->to,
					'company' => $values->company,
					'employee_login' => $this->getUser()->getId(),
					'plane_id' => $values->plane_id,
					'gate_id' => $values->gate_id,
				));
				$other_table_post = $this->database->table('Internal_flight')->get($flightId);
				if($other_table_post)
					$other_table_post->update(array(
						'representing_company' => $values->representing_company,
						'drink' => $values->drink,
					));
				else
				{
					$other_table_post = $this->database->table('International_flight')->get($flightId);
					$other_table_post->update(array(
						'services' => $values->services,
						'movies' => $values->movies,
						'food_list' => $values->food_list,
					));
				}
				#update leteniek pre dany let
				if($this->oldFlightPlaneId != $values->plane_id) {
					$this->database->table('Ticket')->where('flight_id', $flightId)->update(array(
						'plane_id' => $values->plane_id
					));
				}
				$this->flashMessage('Let uspesne upraven '.$typeOfFlight, 'success');
				$this->redirect('this');
			}
			else
			{
				$inserted_flight = $this->database->table('Flight')->insert(array( #$user->getIdentity()->username;
					'departure' => $values->departure,
					'arrival' => $values->arrival,
					'from' => $values->from,
					'to' => $values->to,
					'company' => $values->company,
					'employee_login' => $this->getUser()->getId(),#$this->user->getIdentity()->username,
					'plane_id' => $values->plane_id,
					'gate_id' => $values->gate_id,
				));
				#odeslani do tabulky vnitrno/mezistatnich letu
				#vnitrostatni
				if($typeOfFlight == 0)
				{
					$this->database->table('Internal_flight')->insert(array(
						'flight_id' => $inserted_flight->flight_id,
						'representing_company' => $values->representing_company,
						'drink' => $values->drink,
					));
				}
				#mezistatni
				else
				{
					$this->database->table('International_flight')->insert(array(
						'flight_id' => $inserted_flight->flight_id,
						'services' => $values->services,
						'movies' => $values->movies,
						'food_list' => $values->food_list,
					));
				}
				$this->flashMessage('Let uspesne vlozen', 'success');
				$this->redirect('this');
			}
		}
	}

	private function canPlaneFlyFromGate($planeType, $gate, &$canFlyFrom) {
		$select = $this->database->table('Flying_from')->where('plane_type', $planeType);
		$canFlyFrom = array();
		foreach($select as $row) {
			if($row->gate_id == $gate) {
				return true;
			}
			array_push($canFlyFrom, $row->gate_id);
		}
		return false;
	}

	public function actionEditFlight($flightId)
	{
		if(	!$this->getUser()->isInRole(Model\Role::SUPERVISOR)
		&&	!$this->getUser()->isInRole(Model\Role::MANAGER))
		{
			$this->redirectDeniedAction('DbSearch:searchFlight');
		}
		else
		{
			#cekni jestli existuje let s dannym flight ID a pokud ano uloz
			$flight = $this->database->table('Flight')->get($flightId);
			if (!$flight) {
				$this->error('Let nebyl nalezen');
			}
			#jedna se o flight mezi nebo vnitro statni?
			$flight_type = $this->database->table('Internal_flight')->get($flightId);
			if(!$flight_type)
			{
				$this->typeOfFlightGlobal = 1;
				$flight_type = $this->database->table('International_flight')->get($flightId);
			}
			else
				$this->typeOfFlightGlobal = 0;
			$flight_for_form = array_merge($flight->toArray(),$flight_type->toArray());

			$this['addFlightForm']->setDefaults($flight_for_form);

			$this->template->editFlightId = $flight->flight_id;
			$this->editFlightId = $flightId;
			$this->oldFlightPlaneId = $flight->plane_id;
 		}
	}

	private function redirectDeniedAction($redirect) {
		$this->flashMessage("Nemáte právo na tuhle akci!", "warning");
		$this->redirect($redirect);
	}

	public function actionEditPlane($planeId)
	{
		if(	!$this->getUser()->isInRole(Model\Role::SUPERVISOR)
				&&	!$this->getUser()->isInRole(Model\Role::MANAGER))
		{
			$this->redirectDeniedAction('DbSearch:searchPlane');
		}

		$plane = $this->database->table('Plane')->get($planeId);
		if (!$plane) {
			$this->error('Letadlo nebylo nalezeno');
		}

		$this['addPlaneForm']->setDefaults(array(
					'plane_id' => $plane->plane_id,
					'manufacture_date' => date("Y-m-d", strtotime($plane->manufacture_date)),
					'last_revision' => date("Y-m-d", strtotime($plane->last_revision)),
					'crew_number' => $plane->crew_number,
					'plane_type' => $plane->plane_type,
				)
		);
		$this->template->editPlaneId = $plane->plane_id;

	}

	#sekce pro pridani zamestnance
	public function renderAddEmployee()
	{
		if(!$this->getUser()->isInRole(Model\Role::MANAGER))
		{
			$this->redirectDeniedAction('Menu:mainMenu');
		}
		else
		{
			if (!$this->getUser()->isLoggedIn()) {
				$this->redirect('Homepage:');
			}

			if(!$this->getUser()->isInRole(Model\Role::MANAGER)) {
				$this->redirectDeniedAction('Menu:MainMenu');
			}
		}
	}

	protected function createComponentAddEmployeeForm()
	{
		$role = array();

		#get list of terminals
		$employee = array();
		$employee_db = $this->database->table('Airport_employee');
		foreach ( $employee_db as $item )
		{
			array_push($employee, $item->login);
		}
		#for manager
		if(	$this->getUser()->isInRole(Model\Role::MANAGER))
		{
			$role = array(
				'manager' => 'manager',
				'employee' => 'employee',
				'supervisor' => 'supervisor',
			);
		}
		#for supervisor
		else
		{
			$role = array(
				'employee' => 'employee',
				'supervisor' => 'supervisor',
			);
		}

		$form = new Form;

		$form->addText('login', 'Login:')->setRequired();
		$form->addPassword('password', 'Heslo:');
		$form->addPassword('password_check', 'Heslo znovu:');
		$form->addSelect('role', 'Pozice/role:')->setItems($role, FALSE);
		$form->addSelect('superior_login', 'Login nadrizeneho:')->setItems($employee, FALSE);
		$form->addText('position', 'Nazev pozice:');
		$form->addText('name', 'Jmeno:');
		$form->addText('surname', 'Prijmeni:');
		$form->addText('street', 'Ulice + cislo popisne:');
		$form->addText('city', 'Mesto:');
		$form->addText('postal_code', 'PSC:');
		$form->addText('birth_number', 'Rodne cislo:')->setOption('description', '(Format: RRMMDDXXXX.)');

		$form->addSubmit('send', 'Pridat zamestnance');
		$form->onSuccess[] = array($this, 'addEmployeeFormSucceeded');
		return $form;
	}

	public function addEmployeeFormSucceeded($form, $values)
	{

		$login_check = $this->getParameter('login');
		#check if login is unique
		if(!$login_check)
		{
			$unique_login = $this->database->table('Airport_employee')->get($values->login);
			if($unique_login)
			{
				$this->flashMessage('Chyba! Login jiz existuje!', 'error');
				$this->redirect('this');
			}
		}
		#check passwords match
		if(strcmp($values->password, $values->password_check) != 0)
		{
			$this->flashMessage('Chyba! Hesla nesouhlasi!', 'error');
		}
		#check if name and surname has no numbers
		else if ((preg_match('#[0-9]#',$values->name) && $values->name) || (preg_match('#[0-9]#',$values->surname) && $values->surname))
		{
			$this->flashMessage('Chyba! Neplatne jmeno nebo prijmeni!', 'error');
		}
		else if(!(preg_match('#[0-9]#',$values->street)) && $values->street)
		{
			$this->flashMessage('Chyba! Neplatny format ulice + popisneho cisla!', 'error');
		}
		else if (preg_match('#[0-9]#',$values->city) && $values->city)
		{
			$this->flashMessage('Chyba! Neplatne mesto!', 'error');
		}
		else if (!(preg_match('#^[0-9]{5}$#',$values->postal_code)) && $values->postal_code)
		{
			$this->flashMessage('Chyba! Neplatne PSC!', 'error');
		}
		#rodne cislo check
		else if(!(preg_match('~[0-9][0-9](0[1-9]|1[0-2]|5[1-9]|6[0-2])([0-2][1-9]|3[0-1])([0-9]{4}|[1-9]{3})~', $values->birth_number)) && $values->birth_number)
		{
			$this->flashMessage('Chyba! Neplatne rodne cislo!', 'error');
		}
		else
		{
			if($login_check)
			{
				$post = $this->database->table('Airport_employee')->get($login_check);
				if(!$values->password)
					$values->password = $post->password;

				$post->update(array(
					'login' => $values->login,
					'password' => $values->password,
					'role' => $values->role,
					'superior_login' => $values->superior_login,
					'position' => $values->position,
					'name' => $values->name,
					'surname' => $values->surname,
					'street' => $values->street,
					'city' => $values->city,
					'postal_code' => $values->postal_code,
					'birth_number' => $values->birth_number,
				));
				$this->flashMessage('Zamestnanec uspesne aktualizovan', 'success');
				$this->redirect('this');
			}
			else
			{
				$this->database->table('Airport_employee')->insert(array(
					'login' => $values->login,
					'password' => $values->password,
					'role' => $values->role,
					'superior_login' => $values->superior_login,
					'position' => $values->position,
					'name' => $values->name,
					'surname' => $values->surname,
					'street' => $values->street,
					'city' => $values->city,
					'postal_code' => $values->postal_code,
					'birth_number' => $values->birth_number,
				));

				$this->flashMessage('Zamestnanec uspesne vlozen', 'success');
				$this->redirect('this');
			}
		}
	}
	public function actionEditEmployee($login)
	{
		if(	!$this->getUser()->isInRole(Model\Role::SUPERVISOR)
				&&	!$this->getUser()->isInRole(Model\Role::MANAGER))
		{
			$this->redirectDeniedAction('DbSearch:searchEmployee');
		}
		else
		{
			$employee = $this->database->table('Airport_employee')->get($login);
			if (!$login) {
				$this->error('Zamestanenc nebyl nalezen');
			}
			$this['addEmployeeForm']->setDefaults(array(
						'login' => $employee->login,
						'password' => $employee->password,
						'role' => $employee->role,
						'superior_login' => $employee->superior_login,
						'position' => $employee->position,
						'name' => $employee->name,
						'surname' => $employee->surname,
						'street' => $employee->street,
						'city' => $employee->city,
						'postal_code' => $employee->postal_code,
						'birth_number' => $employee->birth_number,
					)
			);

		}
	}

	#sekce s terminaly
	public function renderAddTerminal()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}

		if(!$this->getUser()->isInRole(Model\Role::MANAGER)) {
			$this->redirectDeniedAction('Menu:MainMenu');
		}
	}

	protected function createComponentAddTerminalForm()
	{
		$terminalType = array(
			'Passenger-Departures' => 'Passenger-Departures',
			'Cargo-Arrivals' => 'Cargo-Arrivals',
		);

		$form = new Form;

		$form->addText('terminal_id', 'ID:')->setRequired();
		$form->addTextArea('services', 'Nabizene sluzby:')->setOption('description', "‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ Jednotlive sluzby nabizene v terminalu oddelene carkou");
		$form->addSelect('terminal_type', 'Typ:')->setItems($terminalType, FALSE);


		$form->addSubmit('send', 'Pridat terminal');
		$form->onSuccess[] = array($this, 'addTerminalFormSucceeded');
		return $form;
	}

	public function addTerminalFormSucceeded($form, $values)
	{
		#check if login is unique
		$unique_terminal = $this->database->table('Terminal')->get($values->terminal_id);
		if($unique_terminal)
		{
			$this->flashMessage('Chyba! Terminal jiz existuje!', 'error');
		}
		else
		{

			$this->database->table('Terminal')->insert(array(
				'terminal_id' => $values->terminal_id,
				'services' => $values->services,
				'terminal_type' => $values->terminal_type,
			));

			$this->flashMessage('Terminal uspesne vlozen', 'success');
			$this->redirect('this');
		}
	}

	#sekce s tickety
	public function renderAddTicket()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
	}

	protected function createComponentAddTicketForm()
	{
		#get list of flights
		$flights = array();
		$flights_db = $this->database->table('Flight');
		foreach ( $flights_db as $item )
		{
			$string = "ID: <".$item->flight_id."> Z: ".$item->from." Do: ".$item->to." Odlet: ".$item->departure;
			array_push($flights, $string);
		}

		$form = new Form;

		$form->addSelect('flight', 'Lety:')->setItems($flights, FALSE)->setOption('description', " ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌  ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌  ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ ‌‌ Vyberte prosim let");



		$form->addSubmit('send', 'Vybrat let');
		$form->onSuccess[] = array($this, 'addTicketFormSucceeded');
		return $form;
	}

	public function addTicketFormSucceeded($form, $values)
	{
		$flight_id = preg_replace("~.*<([0-9]+)>.*~", "$1", $values->flight);;
		$this->redirect('DbAdd:addTicket2', $flight_id);
	}
	#second part of tickets - after choosing flight we can add more info
	public function actionAddTicket2 ($flight_id) {
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->flight_id = $flight_id;
	}
	public function renderAddTicket2()
	{
		if (!$this->getUser()->isLoggedIn()) {
			$this->redirect('Homepage:');
		}
		$this->template->flight_id = $this->flight_id;
	}

	protected function createComponentAddTicket2Form()
	{
		#get flight id
		$flight_id = $this->getParameter('flight_id');
		#get plane type
		$flights_db = $this->database->table('Flight')->get($flight_id);
		$plane_id = $flights_db->plane_id;
		#get list of seats
		$firstClassSeats = array();
		$secondClassSeats = array();
		$others = array();
		$allSeats = $this->getFreeSeats($flight_id, $plane_id);
		foreach($allSeats as $seat) {
			$seatClass = $seat->class;
			switch ($seatClass) {
				case 1:
					array_push($firstClassSeats, $seat->seat_number."\t(Sector: ".$seat->sector.")");
					break;
				case 2:
					array_push($secondClassSeats, $seat->seat_number."\t(Sector: ".$seat->sector.")");
					break;
				default:
					array_push($others, $seat->seat_number."\t(Sector: ".$seat->sector.")");
					break;
			}
		}
		$seats = array(
				'1. trieda' => $firstClassSeats,
				'2. trieda' => $secondClassSeats,
				$others
		);

		#create form
		$form = new Form;

		$form->addText('passenger_name', 'Jmeno pasazera:');
		$form->addText('passenger_surname', 'Prijmeni pasazera:');
		$form->addText('passenger_email', 'Email pasazera:');
		$form->addText('passport_number', 'Cislo pasu:')->setOption('description', "(1-11 cisel)");
		$form->addSelect('seat_number', 'Cislo sedadla:')->setItems($seats, FALSE);
		$form->addText('ticket_price', 'Cena letenky:')->setOption('description', "(Pouze cisla)");



		$form->addSubmit('send', 'Pridat listek');
		$form->onSuccess[] = array($this, 'addTicket2FormSucceeded');
		return $form;
	}

	public function addTicket2FormSucceeded($form, $values)
	{
		#get flight id
		$flight_id = $this->getParameter('flight_id');
		#get plane type
		$flights_db = $this->database->table('Flight')->get($flight_id);
		$plane_id = $flights_db->plane_id;
		if(preg_match('~[0-9]~', $values->passenger_name) && $values->passenger_name)
		{
			$this->flashMessage('Chyba! Neplatne jmeno!', 'error');
		}
		else if(preg_match('~[0-9]~', $values->passenger_surname) && $values->passenger_surname)
		{
			$this->flashMessage('Chyba! Neplatne prijmeni!', 'error');
		}
		else if(!preg_match('~.*@.*\..*~', $values->passenger_email) && $values->passenger_email)
		{
			$this->flashMessage('Chyba! Neplatny email!', 'error');
		}
		else if(!preg_match('~^[0-9]{1,11}$~', $values->passport_number) && $values->passport_number)
		{
			$this->flashMessage('Chyba! Neplatny pas!', 'error');
		}
		else if(!preg_match('~^[0-9]+$~', $values->ticket_price) && $values->ticket_price )
		{
			$this->flashMessage('Chyba! Neplatna cena!', 'error');
		}
		else
		{
			$this->database->table('Ticket')->insert(array(
				'passenger_name' => $values->passenger_name,
				'passenger_surname' => $values->passenger_surname,
				'passenger_email' => $values->passenger_email,
				'passport_number' => $values->passport_number,
				'boarding_pass' => 'F',
				'flight_id' => $flight_id,
				'seat_number' => $values->seat_number,
				'plane_id' => $plane_id,
				'ticket_price' => $values->ticket_price,
			));

			$this->flashMessage('Listek uspesne vlozen', 'success');
		}
	}

	public function getFreeSeats($flight_id, $plane_id) {
		$queryString = sprintf("SELECT S.* FROM (SELECT * FROM Seat WHERE plane_id='%s') AS S LEFT JOIN (SELECT * FROM Ticket WHERE flight_id=$flight_id) AS T ON T.seat_number = S.seat_number WHERE ticket_id IS NULL", $plane_id);
		return $this->database->query($queryString);
	}

	public function actionGiveTicket($ticket_id) {
		if($this->user->isLoggedIn()){
			$this->ticket_id = $ticket_id;
			$this->database->table('Ticket')->where('ticket_id', $ticket_id)
					->update(array('boarding_pass' => 'T'));
		}
	}

	public function renderGiveTicket() {
		if(!$this->user->isLoggedIn()){
			$this->redirect('Homepage:');
		}
		Debugger::barDump($this->ticket_id);
		$this->template->ticket = $this->database->table('Ticket')->where('ticket_id', $this->ticket_id)->fetch();
		Debugger::barDump($this->template->ticket);
	}
}
