<?php
/**
 * Created by PhpStorm.
 * User: martin
 * Date: 29.11.2015
 * Time: 11:54
 */

namespace App\Presenters;


use Latte\RegexpException;
use Nette\Application\UI\Form;
use Nette\Application\UI\Presenter;
use Nette\Database\Context;
use Tracy\Debugger;

class DbSearchPresenter extends Presenter
{
    private $database;
    private $selection;
    private $internationalFlight = false;
    private $internalFlight = false;
    private $flight_id;

    public function __construct(Context $database)
    {
        parent::__construct();
        $this->database = $database;
    }

    public function renderSearchPlane() {
        if(!$this->user->isLoggedIn()){
            $this->redirect('Homepage:');
        }

        if(isset($this->selection)){
            $this->template->planes = $this->selection;
        }
        else {
            $this->template->planes = $this->database->table('Plane')
                ->select('Plane.*,
                 Plane_type.manufacturer,
                 Plane_type.number_of_engines,
                 Plane_type.width,
                 Plane_type.height,
                 Plane_type.lenght');
        }
    }

    protected function createComponentSearchPlaneForm()
    {
        $planeTypes = array();
        $planeManufacturer = array();
        $planeTypes_db = $this->database->table('Plane_type');
        foreach ( $planeTypes_db as $item )
        {
            array_push($planeTypes, $item->plane_type);
            if(!in_array($item->manufacturer, $planeManufacturer) && !empty($item->manufacturer)) {
                array_push($planeManufacturer, $item->manufacturer);
            }
        }

        $form = new Form;
        $form->addSelect('plane_type', 'Typ letadla:')
            ->setItems($planeTypes, false)
            ->setPrompt('-');
        $form->addSelect('manufacturer', 'Vyrobca:')
            ->setItems($planeManufacturer, false)
            ->setPrompt('-');
        $form->addText('plane_id', 'ID letadla:');

        $form->addText('manufacture_date_from', 'Datum vyroby od:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('manufacture_date_to', 'Datum vyroby do:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('last_revision_from', 'Datum posledni revize od:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('last_revision_to', 'Datum posledni revize do:')
            ->setOption('description', '(RRRR-MM-DD format)');

        $form->addSubmit('send', 'Vyhledat letadlo');
        $form->onSuccess[] = array($this, 'searchPlaneFormSucceeded');
        return $form;
    }

    public function searchPlaneFormSucceeded($form, $values)
    {
        $selection = $this->database->table('Plane');

        if(isset($values->plane_type)) {
            $selection = $selection = $selection->where('Plane.plane_type', $values->plane_type);
        }
        if(isset($values->manufacturer) && !empty($values->manufacturer)) {
            $selection = $selection->where('Plane_type.manufacturer', $values->manufacturer);
        }
        if(isset($values->plane_id) && !empty($values->plane_id)) {
            $selection = $selection->where('plane_id', $values->plane_id);
        }
        if(isset($values->manufacture_date_from) && !empty($values->manufacture_date_from)) {
            if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->manufacture_date_from)) {
                $selection = $selection->where('manufacture_date >= ?', $values->manufacture_date_from);
            }
            else {
                $this->flashMessage('Spatny format data vyroby!', 'info');
            }
        }
        if(isset($values->manufacture_date_to) && !empty($values->manufacture_date_to)) {
                        if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->manufacture_date_to)) {
                $selection = $selection->where('manufacture_date <= ?', $values->manufacture_date_to);
            }
            else {
                $this->flashMessage('Spatny format data vyroby!', 'info');
            }
        }
        if(isset($values->last_revision) && !empty($values->last_revision)) {
                        if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->last_revision)) {
                $selection = $selection->where('last_revision', $values->last_revision);
            }
            else {
                $this->flashMessage('Spatny format data posledni revize!', 'info');
            }
        }
        $selection = $selection->select('Plane.*,
                 Plane_type.manufacturer,
                 Plane_type.number_of_engines,
                 Plane_type.width,
                 Plane_type.height,
                 Plane_type.lenght');
        $this->selection = $selection;
    }

    public function renderSearchFlight() {
        if(!$this->user->isLoggedIn()){
            $this->redirect('Homepage:');
        }

        if(isset($this->selection)){
            $this->template->flights = $this->selection;
        }
        else {
            $this->template->flights = $this->database->table('Flight');
        }
        $this->template->internalFlight = $this->internalFlight;
        $this->template->internationalFlight = $this->internationalFlight;
    }

    protected function createComponentSearchFlightForm()
    {
        $flightTypes = array();
        array_push($flightTypes, 'Vnitrostatni');
        array_push($flightTypes, 'Medzistatni');

        $flight_table = $this->database->table('Flight');
        $arr1 = array();
        $flights_in = $this->database->table('Internal_flight');
        foreach ($flights_in as $row) {
            array_push($arr1, $row->flight_id);
        }
        $arr2 = array();
        $flights_out = $this->database->table('International_flight');
        foreach ($flights_out as $row) {
            array_push($arr2, $row->flight_id);
        }
        $flights = array(
            'Vnitrostatni' => $arr1,
            'Medzistatni' => $arr2
        );

        $planes = array();
        $companies = array();
        $gates = array();
        foreach ($flight_table as $fl) {
            if (!in_array($fl->plane_id, $planes)) array_push($planes, $fl->plane_id);
            if (!in_array($fl->company, $companies) && !empty($fl->company)) array_push($companies, $fl->company);
            if (!in_array($fl->gate_id, $gates)) array_push($gates, $fl->gate_id);
        }

        $form = new Form;
        $form->addSelect('flight_type', 'Typ letu:')
            ->setItems($flightTypes, false)
            ->setRequired();

        $form->addSelect('flight_id', 'Cislo letu:')
            ->setItems($flights, false)
            ->setPrompt('-');
        $form->addText('departure_from', 'Datum odletu od:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('departure_to', 'Datum odletu do:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('arrival_from', 'Datum priletu od:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('arrival_to', 'Datum priletu do:')
            ->setOption('description', '(RRRR-MM-DD format)');
        $form->addText('from', 'Miesto odletu:');
        $form->addText('to', 'Miesto priletu:');


        $form->addSelect('plane_id', 'ID letadla:')
            ->setItems($planes, false)
            ->setPrompt('-');
        $form->addSelect('company', 'Spolecnost:')
            ->setPrompt('-')
            ->setItems($companies, false);
        $form->addSelect('gate_id', 'Gate:')
            ->setPrompt('-')
            ->setItems($gates, false);

        $form->addSubmit('send', 'Vyhledat let');
        $form->onSuccess[] = array($this, 'searchFlightFormSucceeded');
        return $form;
    }

    public function searchFlightFormSucceeded($form, $values)
    {
        $selection = $this->database->table('Flight');
        $internal_flight = false;
        $international_flight = false;
        if($values->flight_type == 'Vnitrostatni') {
            $selection = $this->database->table('Internal_flight');
            $selection = $selection->select('Internal_flight.*,
                                Flight.*');
            $internal_flight = true;
        }
        else if($values->flight_type == 'Medzistatni') {
            $selection = $this->database->table('International_flight');
            $selection = $selection->select('International_flight.*,
                                Flight.*');
            $international_flight = true;
        }

        if(isset($values->flight_id) && !empty($values->flight_id)) {
            $selection = $selection->where('Flight.flight_id', $values->flight_id);
        }
        /** Datumy **/
        if(isset($values->departure_from) && !empty($values->departure_from)) {
            if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->departure_from)) {
                $selection = $selection->where('departure >= ?', $values->departure_from);
            }
            else {
                $this->flashMessage('Spatny format data odletu!', 'info');
            }
        }
        if(isset($values->departure_to) && !empty($values->departure_to)) {
            if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->departure_to)) {
                $selection = $selection->where('departure <= ?', $values->departure_to);
            }
            else {
                $this->flashMessage('Spatny format data odletu!', 'info');
            }
        }
        if(isset($values->arrival_from) && !empty($values->arrival_from)) {
            if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->arrival_from)) {
                $selection = $selection->where('arrival >= ?', $values->arrival_from);
            }
            else {
                $this->flashMessage('Spatny format data priletu!', 'info');
            }
        }
        if(isset($values->arrival_to) && !empty($values->arrival_to)) {
            if(preg_match('/\d{4}-\d{2}-\d{2}/', $values->arrival_to)) {
                $selection = $selection->where('arrival <= ?', $values->arrival_to);
            }
            else {
                $this->flashMessage('Spatny format data priletu!', 'info');
            }
        }
        /** Miesta **/
        if(isset($values->from) && !empty($values->from)){
            $selection = $selection->where('from' , $values->from);
        }
        if(isset($values->to) && !empty($values->to)){
            $selection = $selection->where('to' , $values->to);
        }
        /* Zvysok */
        if(isset($values->plane_id) && !empty($values->plane_id)){
            $selection = $selection->where('plane_id', $values->plane_id);
        }
        if(isset($values->company) && !empty($values->company)){
            $selection = $selection->where('company', $values->company);
        }
        if(isset($values->gate_id) && !empty($values->gate_id)){
            $selection = $selection->where('gate_id', $values->gate_id);
        }

        $this->internationalFlight = $international_flight;
        $this->internalFlight = $internal_flight;
        $this->selection = $selection;
    }

    public function renderSearchEmployee() {
        if(!$this->user->isLoggedIn()){
            $this->redirect('Homepage:');
        }

        if(isset($this->selection)){
            $this->template->employees = $this->selection;
        }
        else {
            $this->template->employees = $this->database->table('Airport_employee');
        }
    }

    protected function createComponentSearchEmployeeForm() {
        $form = new Form();

        $form->addText('login', 'Prihlasovacie meno:');
        $form->addText('name', 'Meno:');
        $form->addText('surname', 'Prijmeni:');
        $form->addText('position', 'Pozice:');

        $form->addSubmit('send', 'Vyhledat zamestnance');
        $form->onSuccess[] = array($this, 'searchEmployeeFormSucceeded');
        return $form;
    }

    public function searchEmployeeFormSucceeded($form, $values) {
        $selection = $this->database->table('Airport_employee');

        if(isset($values->login) && !empty($values->login)) {
            $selection = $selection->where('login', $values->login);
        }
        if(isset($values->name) && !empty($values->name)) {
            $selection = $selection->where('name LIKE ?', $values->name.'%');
        }
        if(isset($values->surname) && !empty($values->surname)) {
            $selection = $selection->where('surname LIKE ?', $values->surname.'%');
        }
        if(isset($values->position) && !empty($values->position)) {
            $selection = $selection->where('position LIKE ?', '%'.$values->position.'%');
        }

        $this->selection = $selection;
    }

    public function actionSearchTicket($flight_id) {
        if(isset($flight_id) && !empty($flight_id)) {
            $this->flight_id = $flight_id;
        }
    }

    public function renderSearchTicket() {
        if(!$this->user->isLoggedIn()){
            $this->redirect('Homepage:');
        }
        $this->template->flight_id = $this->flight_id;

        if(isset($this->selection)){
            $this->template->tickets = $this->selection;
        }
        else if(isset($this->flight_id)) {
            $this->template->tickets = $this->database->table('Ticket')
                ->where('flight_id', $this->flight_id);
        }
        else {
            $this->template->tickets = $this->database->table('Ticket');
        }
    }

    protected function createComponentSearchTicketForm() {
        $form = new Form();

        $form->addText('ticket_id', 'Cislo letenky:');
        $form->addText('passenger_name', 'Meno cestujiceho:');
        $form->addText('passenger_surname', 'Prijmeni cestujiciho:');
        if(!isset($this->flight_id)) {
            $form->addText('flight_id', 'Cislo letu:');
        }

        $form->addSubmit('send', 'Vyhledat letenku');
        $form->onSuccess[] = array($this, 'searchEmployeeFormSucceeded');
        return $form;
    }

    public function searchTicketFormSucceeded($form, $values) {

        $selection = $this->database->table('Ticket');

        if(isset($values->ticket_id) && !empty($values->ticket_id)) {
            if(is_int($values->ticket_id)) {
                $selection = $selection->where('ticket_id', $values->ticket_id);
            }
            else {
                $this->flashMessage('Cislo letu nie je cislo');
            }
        }
        if(isset($values->passenger_name) && !empty($values->passenger_name)) {
            $selection = $selection->where('passenger_name LIKE ?', $values->passenger_name.'%');
        }
        if(isset($values->passenger_surname) && !empty($values->passenger_surname)) {
            $selection = $selection->where('passenger_surname LIKE ?', $values->passenger_surname.'%');
        }
        if(isset($values->flight_id) && !empty($values->flight_id)) {
            if(is_int($values->flight_id)) {
                $selection = $selection->where('flight_id', $values->flight_id);
            }
            else {
                $this->flashMessage('Cislo letu nie je cislo');
            }
        }
        if(isset($this->flight_id)) {
            $selection = $selection->where('flight_id', $this->flight_id);
        }

        $this->selection = $selection;
    }
}