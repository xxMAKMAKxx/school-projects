package ija.bluerock.labyrinth.game;

import ija.bluerock.labyrinth.game.board.MazeBoard;
import static ija.bluerock.labyrinth.game.board.MazeBoard.createMazeBoard;
import ija.bluerock.labyrinth.game.board.MazeCard;
import static ija.bluerock.labyrinth.game.board.MazeCard.*;
import ija.bluerock.labyrinth.game.board.MazeField;
import ija.bluerock.labyrinth.command.Command;
import ija.bluerock.labyrinth.command.CommandManager;
import ija.bluerock.labyrinth.game.player.Player;
import ija.bluerock.labyrinth.game.treasure.CardPack;
import ija.bluerock.labyrinth.game.treasure.TreasureCard;

import java.io.*;

/** Trieda Game má nastarosti logiku celej hry.
 * Jej funkciou je riadiť celý chod hry. Slúži
 * aj ako rozhranie pre GUI. GUI volá funkcie
 * vačšinou len z tejto triedy.
 *
 *
 * @author Martin Kocour
 */
public class Game implements Serializable {
    private transient CommandManager cmdManager;

    private int boardSize;
    private MazeBoard board;

    private int numberOfPlayers;
    private Player[] players;
    private int playerOnTurn; /* hrac na tahu */

    private int numberOfTreasures;
    private CardPack pack;

    private int forbiddenShiftR;
    private int forbiddenShiftC;
    /**
     * Booleovská premenná, ktorá indikuje, či bol šift použitý.
     */
    public boolean useShift;

    private int treasuresToWin;

    /**
     * Vytvorí a nainicializuje inštanciu triedy Game.
     * @see Game
     */
    public Game() {
        this.boardSize = 0;
        this.numberOfPlayers = 0;
        this.numberOfTreasures = 0;
        this.board = null;
        this.forbiddenShiftC = -1;
        this.forbiddenShiftR = -1;
        this.treasuresToWin = 0;
        this.useShift = false;
        this.cmdManager = new CommandManager();
    }

    /**
     * Metoda vytvori novú hru.
     *
     * @param size      Veľkost hracej plochy (5-11).
     * @param players   Počet hracov (2-4).
     * @param treasures Mnozstvo úlohových kartičiek (12/24).
     */
    public void newGame(int size, int players, int treasures) {
        this.boardSize = size;
        this.numberOfPlayers = players;
        this.numberOfTreasures = treasures;

        this.board = createMazeBoard(size);
        board.newGame(treasures);

        this.pack = new CardPack(treasures, treasures);
        pack.shuffle();

        createPlayers(players);
        this.playerOnTurn = this.players[0].getID();

        this.forbiddenShiftC = -1;
        this.forbiddenShiftR = -1;
        this.useShift = false;

        this.treasuresToWin = treasures / players;
    }

    /**
     * Vytvorí noých hráčov a umiestni ich do rohov hracej plochy.
     *
     * @param players   Počet hráčov.
     */
    private void createPlayers(int players) {
        pack.shuffle();
        this.players = new Player[players];

        for (int id=1; id <= players; id++) {
            MazeField mf;
            TreasureCard card = pack.popCard();
            switch (id) {
                case 1:
                    mf = board.get(1, 1);
                    this.players[0] = new Player(id, mf, card, pack);
                    break;
                case 2:
                    mf = board.get(boardSize, boardSize);
                    this.players[1] = new Player(id, mf, card, pack);
                    break;
                case 3:
                    mf = board.get(1, boardSize);
                    this.players[2] = new Player(id, mf, card, pack);
                    break;
                default:
                    mf = board.get(boardSize, 1);
                    this.players[3] = new Player(id, mf, card, pack);
                    break;
            }
        }
    }

    /**
     * Ukladá hru. Na ukladanie hry sa používa rozhranie Serializable.
     *
     * @param SaveID        Identifikátor ukladacieho slotu (1-3).
     * @throws IOException  Ak sa nedá ukladáci súbor otvoriť.
     * @see IOException
     * @see Serializable
     */
    public void saveGame(Integer SaveID) throws IOException {
        String dirName = "saves";
        File dir = new File(dirName);
        if(!dir.isDirectory()){
            dir.mkdir();
        }
        String fileName = dirName + "/" + SaveID.toString() + ".mk";
        File saveFile = new File(fileName);
        FileOutputStream fos = new FileOutputStream(saveFile, false);
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(this);
        fos.close();
    }

    /**
     * Načítava uloženú hru zo súboru.
     *
     * @param loadID                    Identifikátor načítavacieho slotu
     * @return                          Načítanú hru.
     * @throws IOException              Ak súbor neexistuje.
     * @throws ClassNotFoundException   Ak trieda neexistuje.
     * @see ClassNotFoundException
     */
    public static Game loadGame(Integer loadID) throws IOException, ClassNotFoundException {
        String dirName = "saves";
        String fileName = dirName + "/" + loadID.toString() + ".mk";
        FileInputStream fis = new FileInputStream(fileName);
        ObjectInputStream ois = new ObjectInputStream(fis);
        Game game = (Game)ois.readObject();
        fis.close();
        game.cmdManager = new CommandManager();
        return game;
    }

    /**
     * Metoda vracia späť vykonané kroky.
     * Vracia hru do predchádzajúceho stavu.
     *
     * @see CommandManager
     */
    public void undo() {
        cmdManager.undo();
    }

    /**
     * Metoda uľahčuje prístup k hracík kartám.
     * R a C sú súradnice požadovanej karty.
     * Metóda môže vracať null, ak karta neexistuje.
     *
     * @param r Riadok na hracej ploche.
     * @param c Stĺpec na hracej ploche.
     * @return Vracia objekt typu MazeCard.
     * @see MazeCard
     */
    public MazeCard getMazeCard(int r, int c) {
        if (r == -1 && c == -1)
            return getFreeCard();
        else
            return board.get(r, c).getCard();
    }

    /**
     * Metoda vyhladá kartu a vráti kód úlohy na danej karte.
     *
     * @param r Riadok (R-súradnica) na hracej ploche
     * @param c Stĺpec (C-súradnica) na hracej ploche
     * @return Vracia kód úlohy.
     */
    public int getTreasureCode(int r, int c) {
        TreasureCard card;
        if (r == -1 && c == -1)
            card = board.getFreeCard().getTresaureCard();
        else
            card = board.get(r, c).getCard().getTresaureCard();

        if (card != null) {
            return card.getTreasure().getCode();
        }
        else
            return -1;
    }

    /**
     * Metoda nájde a identifikuje všetkých hráčov na danej karte pomocou
     * metody
     * {@link ija.bluerock.labyrinth.game.Game#playersHere(int, int)}.
     * V prípade že sa na danej pozícii vyskytuje viac hráčov, metoda
     * vracia ID hráča, ktorý je aktuálne na ťahu alebo prvého nájdeného.
     *
     * @param r Riadok (R-súradnica) na hracej ploche
     * @param c Stĺpec (C-súradnica) na hracej ploche
     * @return Vracia ID hráča na danej pozícii.
     */
    public int getPlayerHere(int r, int c) {
        Player[] p = playersHere(r, c);
        int playerHereID = -1;
        for (int id = 1; id <= numberOfPlayers; id++) {
            if (p[id-1] != null) {
                playerHereID = id;
                if (id == playerOnTurn) {
                    return id;
                }
            }
        }
        return playerHereID;
    }

    /**
     * Metoda nájde a indetifikuje všetkých hráčov na danej karte.
     *
     * @param r Riadok (R-súradnica) na hracej ploche
     * @param c Stĺpec (C-súradnica) na hracej ploche
     * @return Pole hráčov na danej pozícii.
     */
    public Player[] playersHere(int r, int c) {
        MazeField mf = board.get(r, c);
        Player[] p = new Player[numberOfPlayers];
        for(int i = 0; i < numberOfPlayers; i++) {
            MazeField playerPosition = players[i].getPosition();
            if (mf == playerPosition) {
                p[i] = players[i];
            }
            else {
                p[i] = null;
            }
        }
        return p;
    }

    /**
     *
     * @return Vracia volný kameň.
     */
    public MazeCard getFreeCard() {
        return board.getFreeCard();
    }

    /**
     * Posúva kamene bludiska, smer posunu udáva R a C súradnice.
     * V ďalšom ťahu hráč nesmie posunút hracie políčka
     * v opačnom smere ako boli posunuté v predchadzajucom kroku.
     * V jednom kroku m
     *
     * @param R musi byt z interavlu (1, N)
     * @param C musi byt z interavlu (1, N)
     */
    public void shiftCard(int R, int C) {
        if (!useShift) {
            if (R != forbiddenShiftR || C != forbiddenShiftC) {
                MazeField mf = board.get(R, C);
                cmdManager.executeCmd(new ShiftCommand(this, mf, R, C));
            }
        }
    }

    private class ShiftCommand implements Command {
        private boolean useShift;
        private int previousForbiddenC;
        private int previousForbiddenR;
        private MazeField mf;
        private Game game;
        private int C;
        private int R;

        private ShiftCommand(Game game, MazeField mf, int R, int C) {
            this.mf = mf;
            this.R = R;
            this.C = C;
            this.game= game;
            this.previousForbiddenR = game.forbiddenShiftR;
            this.previousForbiddenC = game.forbiddenShiftC;
            this.useShift = game.useShift;
        }

        @Override
        public void execute() {
            CANGO dir = field2shiftDirection(mf);
            game.useShift = true;
            game.board.shift(mf);
            shiftPlayers(R, C, dir);

            if (R == 1 || R == boardSize) {
                game.forbiddenShiftR = R == 1 ? boardSize : 1;
                game.forbiddenShiftC = C;
            }
            if (C == 1 || C == boardSize) {
                game.forbiddenShiftC = C == 1 ? boardSize : 1;
                game.forbiddenShiftR = R;
            }
        }

        @Override
        public void undo() {
            int r = game.forbiddenShiftR;
            int c = game.forbiddenShiftC;

            MazeField reversemf = game.board.get(r, c);
            game.board.shift(reversemf);
            CANGO reverseDir = field2shiftDirection(reversemf);
            shiftPlayers(r, c, reverseDir);

            game.forbiddenShiftR = this.previousForbiddenR;
            game.forbiddenShiftC = this.previousForbiddenC;
            game.useShift = this.useShift;
        }
    }

    private void shiftPlayers(int r, int c, CANGO dir) {
        Player[] pHere;
        int i;
        int limit;
        int offset;
        if(dir == CANGO.DOWN || dir == CANGO.RIGHT) {
            i = boardSize;
            limit = 0;
            offset = -1;
        }
        else {
            i = 1;
            limit = boardSize+1;
            offset = +1;
        }

        for(; i != limit; i += offset) {
            if(r == 1 || r == boardSize)//DOWN or UP
                pHere = playersHere(i, c);
            else //if(c == 1 || c == boardSize)//LEFT or RIGHT
                pHere = playersHere(r, i);

            for(int j = 0; j < numberOfPlayers; j++){
                if(pHere[j] != null) {
                    MazeField mf = getSurrField(pHere[j].getPosition(), dir);
                    pHere[j].move(mf);
                }
            }
        }
        for (int k = 0; k <numberOfPlayers; k++) {
            if(players[k].getPosition() == null) {
                MazeField mf = board.get(r, c);
                players[k].move(mf);
            }
        }
    }

    /**
     * Posúva hráča po smere dir (VĽAVO, HORE, VPRAVO, DOLE).
     *
     * @param dir   Smer posunu (LEFT, UP, RIGHT, DOWN).
     * @return      Vracia poziciu hráča po posune,
     *              ak sa hráč neposunie, tak vracia
     *              jeho starú poziciu.
     */
    public MazeField movePlayer(CANGO dir) {
        boolean canGoCard = players[playerOnTurn-1].canGo(dir);
        MazeField actualPosition = players[playerOnTurn-1].getPosition();
        if (canGoCard && useShift) {
            MazeField cardToMove = getSurrField(actualPosition, dir);
            if (cardToMove != null) {
                CANGO complementDir = CANGO.complementDirection(dir);
                canGoCard = cardToMove.getCard().canGo(complementDir);
                if (canGoCard) {
                    cmdManager.executeCmd(new MovePlayerCommand(this, actualPosition, cardToMove));
                    actualPosition = cardToMove;
                }
            }
        }
        return actualPosition;
    }

    private class MovePlayerCommand implements Command {
        private Game game;
        private MazeField previousPosition;
        private TreasureCard previousPlayerCard;
        private MazeField newPosition;
        private TreasureCard newPositionTreasure;
        private int score;
        boolean playerFind;

        private MovePlayerCommand(Game game, MazeField actualPos, MazeField cardToMove) {
            this.game = game;
            previousPosition = actualPos;
            previousPlayerCard = game.players[game.playerOnTurn - 1].getTreasureCard();
            newPosition = cardToMove;
            newPositionTreasure = null;
            playerFind = false;
            if(newPosition != null) {
                newPositionTreasure = newPosition.getCard().getTresaureCard();
                if(newPositionTreasure != null &&
                        newPositionTreasure.equals(game.players[game.playerOnTurn-1].getTreasureCard())) {
                    playerFind = true;
                }
            }
            this.score = game.players[game.playerOnTurn-1].getScore();
        }

        @Override
        public void execute() {
            game.players[game.playerOnTurn-1].move(this.newPosition);
        }

        @Override
        public void undo() {
            if(playerFind) {
                newPosition.getCard().putTresaureCard(newPositionTreasure);
                game.players[game.playerOnTurn - 1].putScore(this.score);
                game.players[game.playerOnTurn-1].putTreasureCard(previousPlayerCard);
            }
            game.players[game.playerOnTurn-1].move(this.previousPosition);
        }
    }

    private MazeField getSurrField(MazeField field, CANGO position) {
        MazeField mf;
        int posR = field.row();
        int posC = field.col();
        switch (position) {
            case LEFT:
                mf = board.get(posR, posC-1);
                break;
            case UP:
                mf = board.get(posR-1, posC);
                break;
            case RIGHT:
                mf = board.get(posR, posC+1);
                break;
            default:
                mf = board.get(posR+1, posC);
                break;
        }
        return mf;
    }

    private CANGO field2shiftDirection(MazeField shift) {
        int shiftR = shift.row();
        int shiftC = shift.col();

        if(shiftR == 1)
            return CANGO.DOWN;
        else if(shiftR == boardSize)
            return CANGO.UP;
        else if(shiftC == 1)
            return CANGO.RIGHT;
        else
            return CANGO.LEFT;
    }

    /**
     *
     * @return Vracia hráča na ťahu.
     */
    public Player getActualPlayer() {
        return players[playerOnTurn-1];
    }


    /**
     * Metoda zmení hráča na ťahu.
     */
    public void nextTurn() {
        cmdManager.executeCmd(new NextTurnCommand(this));
    }

    private class NextTurnCommand implements Command {
        Game game;
        int previousPlayerOnTurn;
        TreasureCard previousPlayerTreasure;
        boolean useShift;
        boolean popCard;

        private NextTurnCommand(Game game) {
            this.game = game;
            this.previousPlayerOnTurn = game.playerOnTurn;
            this.useShift = game.useShift;
            this.popCard = false;
            this.previousPlayerTreasure = game.players[game.playerOnTurn-1].getTreasureCard();
        }

        @Override
        public void execute() {
            game.playerOnTurn = game.playerOnTurn % game.numberOfPlayers;
            game.playerOnTurn++;
            game.useShift = false;
            if(game.players[game.playerOnTurn-1].getTreasureCard() == null) {
                //hrac nema ukol, musime mu ho pridelit
                TreasureCard card = game.pack.popCard();
                game.players[game.playerOnTurn-1].putTreasureCard(card);
                this.popCard = true;
            }
        }

        @Override
        public void undo() {
            if(popCard) {
                game.pack.pushCard(game.players[game.playerOnTurn-1].getTreasureCard());
                this.popCard = false;
            }
            game.useShift = this.useShift;
            game.playerOnTurn = this.previousPlayerOnTurn;
            game.players[game.playerOnTurn-1].putTreasureCard(previousPlayerTreasure);
        }
    }

    /**
     * Metoda vrácia kód pokladu, ktorý aktuálny hráč na ťahu má
     * za úlohu nájsť.
     *
     * @return      číslo úlohovej karty-pokladu (0-23)
     */
    public int getActualPlayerTreasure() {
        TreasureCard playerCard = getActualPlayer().getTreasureCard();
        if(playerCard == null)
            return -1;
        else
            return playerCard.getTreasure().getCode();
    }

    /**
     *
     * @param id    ID hráča
     * @return      číslo úlohoovej karty (pokladu) hráča (0-23)
     */
    public int getPlayerTreasure(int id) {
        return players[id-1].getTreasureCard().getTreasure().getCode();
    }

    /**
     * @return Vracia skóre hráčov.
     */
    public Integer[] getPlayersScore() {
        Integer[] scores = new Integer[numberOfPlayers];
        for (int i = 0; i < numberOfPlayers; i++) {
            scores[i] = players[i].getScore();
        }
        return scores;
    }

    /**
     * Metoda sa stará o to, či aktuálny hráč už dosiahol
     * požadované skóre.
     *
     * @return Vracia čislo hráča, ktorý vyhral (1-4) alebo -1.
     */
    public int winner() {
        if (players[playerOnTurn-1].getScore() == treasuresToWin)
            return playerOnTurn;
        return -1;
    }

    /**
     *
     * @return Vracia veľkosť hracej plochy.
     */
    public int getBoardSize() {
        return boardSize;
    }

    /**
     *
     * @return Vracia počet hráčov hry.
     */
    public int getNumberOfPlayers() {
        return numberOfPlayers;
    }
}
