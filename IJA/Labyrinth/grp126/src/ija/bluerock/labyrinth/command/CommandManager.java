package ija.bluerock.labyrinth.command;

import java.util.Stack;

/** Táto trieda sa stará o všetky príkazy, ktoré menia stav hry.
 * Spúšťa tieto prikazy a následne si vedie informácie o stave pred
 * a po spustení. Tieto stavy potom ukladáda do zásobníku.
 * Vďaka tomu potom môžeme vykonáť undo.
 *
 *
 * @author Martin Kocour
 */
public class CommandManager {
    private Stack<Command> undos;
    //redo

    public CommandManager() {
        undos = new Stack<Command>();
    }

    public void executeCmd(Command cmd){
        cmd.execute();
        undos.push(cmd);
        //redo.clear
    }

    public boolean isUndoApplicable() {
        return !undos.empty();
    }

    public void undo() {
        if(!undos.empty()) {
            Command cmd = undos.pop();
            cmd.undo();
        }
    }
}
