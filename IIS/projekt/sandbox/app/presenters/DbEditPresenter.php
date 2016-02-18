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

class DbEditPresenter extends Presenter
{
    private $database;
    private $plane_id;
    private $flight_id;
    private $ticketRow;

    public function __construct(Context $database)
    {
        parent::__construct();
        $this->database = $database;
    }

    public function actionEditSeat($ticket_id) {
        $this->ticketRow = $this->database->table('Ticket')->where('ticket_id', $ticket_id)->fetch();
        $this->plane_id = $this->ticketRow->plane_id;
        $this->flight_id = $this->ticketRow->flight_id;
        $this->template->flight = $this->ticketRow->flight_id;
    }

    protected function createComponentEditSeatForm()
    {
        $firstClassSeats = array();
        $secondClassSeats = array();
        $others = array();
        $allSeats = $this->getFreeSeats($this->flight_id, $this->plane_id);
        foreach($allSeats as $seat) {
            $seatClass = $seat->class;
            switch ($seatClass) {
                case 1:
                    array_push($firstClassSeats, $seat->seat_number."\t".$seat->sector);
                    break;
                case 2:
                    array_push($secondClassSeats, $seat->seat_number."\t".$seat->sector);
                    break;
                default:
                    array_push($others, $seat->seat_number."\t".$seat->sector);
                    break;
            }
        }
        $currentSeat = $this->database->table('Seat')->get(array($this->ticketRow->seat_number, $this->ticketRow->plane_id));
        $seatClass = $currentSeat->class;
        $seatString = $currentSeat->seat_number."\t".$currentSeat->sector;
        switch ($seatClass) {
            case 1:
                array_push($firstClassSeats, $seatString);
                break;
            case 2:
                array_push($secondClassSeats, $seatString);
                break;
            default:
                array_push($others, $seatString);
                break;
        }
        $seats = array(
            '1. trieda' => $firstClassSeats,
            '2. trieda' => $secondClassSeats,
            $others
        );
        $form = new Form();
        $form->addSelect('seat_number', 'Sedadlo')->setItems($seats, false)->setDefaultValue($seatString);
        $form->addSubmit('send', 'Zmenit sedadlo');
        $form->onSuccess[] = array($this, 'editSeatFormSucceeded');
        return $form;
    }

    public function editSeatFormSucceeded($form, $values) {
        if(isset($values->seat_number) && !empty($values->seat_number)) {
            $this->ticketRow->update(array(
                'seat_number' => $values->seat_number
            ));
        }
        $this->flashMessage('Sedadlo uspene zmeneno');
        $this->redirect('DbSearch:searchTicket');
    }

    public function getFreeSeats($flight_id, $plane_id) {
        $queryString = sprintf("SELECT S.* FROM (SELECT * FROM Seat WHERE plane_id='%s') AS S LEFT JOIN (SELECT * FROM Ticket WHERE flight_id=$flight_id) AS T ON T.seat_number = S.seat_number WHERE ticket_id IS NULL", $plane_id);
        return $this->database->query($queryString);
    }

    public function actionDeleteEmployee($login) {
        if($this->getUser()->isInRole('manager') || $this->getUser()->isInRole('supervisor')) {
            $rows = $this->database->table('Airport_employee')->where('login', $login)->delete();
            if($rows <= 0){
                $this->flashMessage("Uzivatele: ".$login." se nepodarilo smazat");
                $this->redirect('DbSearch:searchEmployee');
            }
        }
        else{
            $this->flashMessage("Nemate pravo na tuhle akci");
            $this->redirect('Menu:mainMenu');
        }
    }
}