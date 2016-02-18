package ija.bluerock.labyrinth.command;

/** Rozhranie reprezentuje príkaz, ktorý može byť buď prevedený
 * alebo vrátený spät do predchádzajúceho stavu.
 *
 * @author Martin Kocour
 */
public interface Command {
    void execute();
    void undo();
}
