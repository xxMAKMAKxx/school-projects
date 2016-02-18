package ija.bluerock.labyrinth.gui;

import ija.bluerock.labyrinth.game.board.MazeCard;
import ija.bluerock.labyrinth.game.Game;

import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

/**
 * Třída starající se o vykreslení celé hry.
 * Třída vykresluje boční panely i panel se hrou.
 * Stará se o vykreslení správného rozměru, umístění prvků a celkově celé ovládání hry.
 * Neobsahuje ovšem stavy, které přebírá ze třídy Game.
 * @author Martin Kačmarčík
 */
public class GameWindow extends javax.swing.JFrame {
    private Game game;
    private int rozmer;
    private int numberOfPlayers;
    private int actualHero = 1;
    private boolean visible = false;
    private MenuPanel menu;
    private boolean thisTurnShift = false;
    /* pokus o optimalizaci */
    public static BufferedImage lko1;
    public static BufferedImage lko2;
    public static BufferedImage lko3;
    public static BufferedImage lko4;
    public static BufferedImage rovna1;
    public static BufferedImage rovna2;
    public static BufferedImage tecko1;
    public static BufferedImage tecko2;
    public static BufferedImage tecko3;
    public static BufferedImage tecko4;
    public static BufferedImage[] treasureArray = new BufferedImage[24];
    public static BufferedImage[] playersArray = new BufferedImage[4];
    public static ImageIcon upArrow;
    public static ImageIcon downArrow;
    public static ImageIcon leftArrow;
    public static ImageIcon rightArrow;
    
    
    /**
     * Metoda, ktera inicializuje grafiku do pameti, aby se nemusela vzdy znovu nacitat pri hre.
     * @throws IOException Neprepokladam IOException, proto throw. Slo by osetrit nejakym warningem.
     */
    private void initGraphic() throws IOException{
        lko1 = ImageIO.read(getClass().getResource("/images/finalPng/lko1.png"));
        lko2 = ImageIO.read(getClass().getResource("/images/finalPng/lko2.png"));
        lko3 = ImageIO.read(getClass().getResource("/images/finalPng/lko3.png"));
        lko4 = ImageIO.read(getClass().getResource("/images/finalPng/lko4.png"));
        rovna1 = ImageIO.read(getClass().getResource("/images/finalPng/rovna1.png"));
        rovna2 = ImageIO.read(getClass().getResource("/images/finalPng/rovna2.png"));
        tecko1 = ImageIO.read(getClass().getResource("/images/finalPng/tecko1.png"));
        tecko2 = ImageIO.read(getClass().getResource("/images/finalPng/tecko2.png"));
        tecko3 = ImageIO.read(getClass().getResource("/images/finalPng/tecko3.png"));
        tecko4 = ImageIO.read(getClass().getResource("/images/finalPng/tecko4.png"));
        /* nacteni vsech pokladu do pole */
        for(int i = 0; i < 24; i++){
            treasureArray[i] = ImageIO.read(getClass().getResource("/images/poklady/poklad"+(i+1)+".png"));
        }
        for(int j = 0; j < 4; j++){
            playersArray[j] = ImageIO.read(getClass().getResource("/images/heroes/heroes"+(j+1)+".png"));
        }
        upArrow = new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"));
        downArrow = new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"));
        leftArrow = new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"));
        rightArrow = new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"));
    }
    
    
    
    
    
    /**
     * Metoda nastavující viditelnost (spíše neviditelnost) jednotlivým šipkám ve hře.
     */
    private void setButtonOpacity(){
        /*********************************************/
        /* Setting opacity to arrow (shift) buttons */
        /*********************************************/
        if(rozmer == 11){
            /* right arrow1 set image */
            rightArrow1.setBorder(BorderFactory.createEmptyBorder());
            rightArrow1.setContentAreaFilled(false);
            /* right arrow2 set image */
            rightArrow2.setBorder(BorderFactory.createEmptyBorder());
            rightArrow2.setContentAreaFilled(false);
            /* right arrow3 set image */
            rightArrow3.setBorder(BorderFactory.createEmptyBorder());
            rightArrow3.setContentAreaFilled(false);
            /* right arrow4 set image */
            rightArrow4.setBorder(BorderFactory.createEmptyBorder());
            rightArrow4.setContentAreaFilled(false);
            /* right arrow5 set image */
            rightArrow5.setBorder(BorderFactory.createEmptyBorder());
            rightArrow5.setContentAreaFilled(false);
            /* up arrow1 set image */
            upArrow1.setBorder(BorderFactory.createEmptyBorder());
            upArrow1.setContentAreaFilled(false);
            /* up arrow2 set image */
            upArrow2.setBorder(BorderFactory.createEmptyBorder());
            upArrow2.setContentAreaFilled(false);
            /* up arrow3 set image */
            upArrow3.setBorder(BorderFactory.createEmptyBorder());
            upArrow3.setContentAreaFilled(false);
            /* up arrow4 set image */
            upArrow4.setBorder(BorderFactory.createEmptyBorder());
            upArrow4.setContentAreaFilled(false);
            /* up arrow5 set image */
            upArrow5.setBorder(BorderFactory.createEmptyBorder());
            upArrow5.setContentAreaFilled(false);
            /* left arrow1 set image */
            leftArrow1.setBorder(BorderFactory.createEmptyBorder());
            leftArrow1.setContentAreaFilled(false);
            /* left arrow2 set image */
            leftArrow2.setBorder(BorderFactory.createEmptyBorder());
            leftArrow2.setContentAreaFilled(false);
            /* left arrow3 set image */
            leftArrow3.setBorder(BorderFactory.createEmptyBorder());
            leftArrow3.setContentAreaFilled(false);
            /* left arrow4 set image */
            leftArrow4.setBorder(BorderFactory.createEmptyBorder());
            leftArrow4.setContentAreaFilled(false);
            /* left arrow5 set image */
            leftArrow5.setBorder(BorderFactory.createEmptyBorder());
            leftArrow5.setContentAreaFilled(false);
            /* down arrow1 set image */
            downArrow1.setBorder(BorderFactory.createEmptyBorder());
            downArrow1.setContentAreaFilled(false);
            /* down arrow2 set image */
            downArrow2.setBorder(BorderFactory.createEmptyBorder());
            downArrow2.setContentAreaFilled(false);
            /* down arrow3 set image */
            downArrow3.setBorder(BorderFactory.createEmptyBorder());
            downArrow3.setContentAreaFilled(false);
            /* down arrow4 set image */
            downArrow4.setBorder(BorderFactory.createEmptyBorder());
            downArrow4.setContentAreaFilled(false);
            /* down arrow5 set image */
            downArrow5.setBorder(BorderFactory.createEmptyBorder());
            downArrow5.setContentAreaFilled(false);
        }
        else if(rozmer == 9){
            /* up arrow6 set image */
            upArrow6.setBorder(BorderFactory.createEmptyBorder());
            upArrow6.setContentAreaFilled(false);
            /* up arrow2 set image */
            upArrow7.setBorder(BorderFactory.createEmptyBorder());
            upArrow7.setContentAreaFilled(false);
            /* up arrow3 set image */
            upArrow8.setBorder(BorderFactory.createEmptyBorder());
            upArrow8.setContentAreaFilled(false);
            /* up arrow4 set image */
            upArrow9.setBorder(BorderFactory.createEmptyBorder());
            upArrow9.setContentAreaFilled(false);
            /* down arrow1 set image */
            downArrow6.setBorder(BorderFactory.createEmptyBorder());
            downArrow6.setContentAreaFilled(false);
            /* down arrow2 set image */
            downArrow7.setBorder(BorderFactory.createEmptyBorder());
            downArrow7.setContentAreaFilled(false);
            /* down arrow3 set image */
            downArrow9.setBorder(BorderFactory.createEmptyBorder());
            downArrow9.setContentAreaFilled(false);
            /* down arrow4 set image */
            downArrow8.setBorder(BorderFactory.createEmptyBorder());
            downArrow8.setContentAreaFilled(false);
            /* right arrow1 set image */
            rightArrow6.setBorder(BorderFactory.createEmptyBorder());
            rightArrow6.setContentAreaFilled(false);
            /* right arrow2 set image */
            rightArrow7.setBorder(BorderFactory.createEmptyBorder());
            rightArrow7.setContentAreaFilled(false);
            /* right arrow3 set image */
            rightArrow8.setBorder(BorderFactory.createEmptyBorder());
            rightArrow8.setContentAreaFilled(false);
            /* right arrow4 set image */
            rightArrow9.setBorder(BorderFactory.createEmptyBorder());
            rightArrow9.setContentAreaFilled(false);
            /* left arrow1 set image */
            leftArrow7.setBorder(BorderFactory.createEmptyBorder());
            leftArrow7.setContentAreaFilled(false);
            /* left arrow2 set image */
            leftArrow8.setBorder(BorderFactory.createEmptyBorder());
            leftArrow8.setContentAreaFilled(false);
            /* left arrow3 set image */
            leftArrow6.setBorder(BorderFactory.createEmptyBorder());
            leftArrow6.setContentAreaFilled(false);
            /* left arrow4 set image */
            leftArrow9.setBorder(BorderFactory.createEmptyBorder());
            leftArrow9.setContentAreaFilled(false);
        }
        else if(rozmer == 7){
            /* left arrow2 set image */
            leftArrow10.setBorder(BorderFactory.createEmptyBorder());
            leftArrow10.setContentAreaFilled(false);
            /* left arrow3 set image */
            leftArrow11.setBorder(BorderFactory.createEmptyBorder());
            leftArrow11.setContentAreaFilled(false);
            /* left arrow4 set image */
            leftArrow12.setBorder(BorderFactory.createEmptyBorder());
            leftArrow12.setContentAreaFilled(false);
            /* up arrow6 set image */
            upArrow12.setBorder(BorderFactory.createEmptyBorder());
            upArrow12.setContentAreaFilled(false);
            /* up arrow2 set image */
            upArrow10.setBorder(BorderFactory.createEmptyBorder());
            upArrow10.setContentAreaFilled(false);
            /* up arrow3 set image */
            upArrow11.setBorder(BorderFactory.createEmptyBorder());
            upArrow11.setContentAreaFilled(false);
            /* right arrow1 set image */
            rightArrow10.setBorder(BorderFactory.createEmptyBorder());
            rightArrow10.setContentAreaFilled(false);
            /* right arrow2 set image */
            rightArrow11.setBorder(BorderFactory.createEmptyBorder());
            rightArrow11.setContentAreaFilled(false);
            /* right arrow3 set image */
            rightArrow12.setBorder(BorderFactory.createEmptyBorder());
            rightArrow12.setContentAreaFilled(false);
            /* down arrow1 set image */
            downArrow10.setBorder(BorderFactory.createEmptyBorder());
            downArrow10.setContentAreaFilled(false);
            /* down arrow2 set image */
            downArrow11.setBorder(BorderFactory.createEmptyBorder());
            downArrow11.setContentAreaFilled(false);
            /* down arrow3 set image */
            downArrow12.setBorder(BorderFactory.createEmptyBorder());
            downArrow12.setContentAreaFilled(false);
        }
        else if(rozmer == 5){
            /* up arrow6 set image */
            upArrow13.setBorder(BorderFactory.createEmptyBorder());
            upArrow13.setContentAreaFilled(false);
            /* up arrow2 set image */
            upArrow14.setBorder(BorderFactory.createEmptyBorder());
            upArrow14.setContentAreaFilled(false);
            /* down arrow1 set image */
            downArrow13.setBorder(BorderFactory.createEmptyBorder());
            downArrow13.setContentAreaFilled(false);
            /* down arrow2 set image */
            downArrow14.setBorder(BorderFactory.createEmptyBorder());
            downArrow14.setContentAreaFilled(false);
            /* right arrow1 set image */
            rightArrow13.setBorder(BorderFactory.createEmptyBorder());
            rightArrow13.setContentAreaFilled(false);
            /* right arrow2 set image */
            rightArrow14.setBorder(BorderFactory.createEmptyBorder());
            rightArrow14.setContentAreaFilled(false);
            /* left arrow2 set image */
            leftArrow13.setBorder(BorderFactory.createEmptyBorder());
            leftArrow13.setContentAreaFilled(false);
            /* left arrow3 set image */
            leftArrow14.setBorder(BorderFactory.createEmptyBorder());
            leftArrow14.setContentAreaFilled(false);
        }
    }
    /**
     * Konstruktor třídy, který inicializuje veškeré proměnné a provádí počáteční nastavení hry na zákaldě módu.
     * @param menu Obsahuje třídu s menu, aby se do ní hra mohla kdyoliv vrátit.
     * @param rozmerArg Rozměr hracího pole.
     * @param numberOfPlayersArg Počet hráčů.
     * @param numberOfTreasuresArg Počet pokladů.
     * @param mode Mód, signalizující při 1 - Nová hra, při 2 - Načtení staré hry.
     * @param game2 V případě načtení hry obsahuje třídu s načtenou hrou.
     */
    public GameWindow(MenuPanel menu, int rozmerArg, int numberOfPlayersArg, int numberOfTreasuresArg, int mode, Game game2) {
        initComponents();        
        try {
            initGraphic();
        } catch (IOException ex) {
            Logger.getLogger(GameWindow.class.getName()).log(Level.SEVERE, null, ex);
        }
        this.setVisible(true);
        this.menu = menu;
        if(mode == 1){
            this.rozmer = rozmerArg;
            this.numberOfPlayers = numberOfPlayersArg;
            this.game = new Game();
            this.game.newGame(rozmerArg, numberOfPlayersArg, numberOfTreasuresArg);
        }
        else if(mode == 2){
            this.game = game2;
            this.numberOfPlayers = game.getNumberOfPlayers();
            this.rozmer = game.getBoardSize();
            setActualPlayerLabel();
            setScoreBoard();
            repaintAll();
        }
        GameComponent GC = new GameComponent(rozmer, game, 1);
        GameComponent GC2 = new GameComponent(rozmer, game, 2);
        gameWindowPanel.add(GC);
        jPanel4.add(GC2);
        rightPanel.setVisible(true);
        middlePanel.setVisible(true);
        leftPanel.setVisible(true);
        //jPanel5.setVisible(false);
        
        /* adding image to the exit button */
        BufferedImage image;
        image = null;
        try {
            //image = ImageIO.read(new File("./lib/images/exit.png"));
            image = ImageIO.read(getClass().getResource("/images/exit.png"));
        } catch (IOException ex){System.out.println("Working Directory = " +
              System.getProperty("user.dir"));}
        jButton2.setIcon(new ImageIcon(image));
        jButton2.setBorder(BorderFactory.createEmptyBorder());
        jButton2.setContentAreaFilled(false);
        /*save and load button opacity */
        saveButton.setBorder(BorderFactory.createEmptyBorder());
        saveButton.setContentAreaFilled(false);
        loadButton.setBorder(BorderFactory.createEmptyBorder());
        loadButton.setContentAreaFilled(false);        
        /* adding image to help button */
        try {
            //image = ImageIO.read(new File("./lib/images/helpIcon.png"));
            image = ImageIO.read(getClass().getResource("/images/helpIcon.png"));
        } catch (IOException ex){}
        jButton6.setIcon(new ImageIcon(image));
        jButton6.setBorder(BorderFactory.createEmptyBorder());
        jButton6.setContentAreaFilled(false);
        /* call function that will set button opacity */
        setProperShiftingButtons();
        setButtonOpacity();
        setScoreBoard();
        /* init the acutal treasure */
        setActualTreasure();
        actualTreasure.setVisible(false);
        /* set score board */
        
        
        
        this.requestFocusInWindow();
    }

    /**
     * Metoda starající se o správné vykreslení shiftovacích šipek.
     */
    private void setProperShiftingButtons(){
        topShiftPanel.removeAll();
        leftShiftPanel.removeAll();
        bottomShiftPanel.removeAll();
        rightShiftPanel.removeAll();
        if(rozmer == 11){                 
            topShiftPanel.add(playerTop11);      
            playerTop11.add(upArrow1);
            playerTop11.add(upArrow2);
            playerTop11.add(upArrow3);
            playerTop11.add(upArrow4);           
            playerTop11.add(upArrow5);
            leftShiftPanel.add(playerLeft11);
            playerLeft11.add(leftArrow1);
            playerLeft11.add(leftArrow2);
            playerLeft11.add(leftArrow3);
            playerLeft11.add(leftArrow4);           
            playerLeft11.add(leftArrow5);
            bottomShiftPanel.add(playerBottom11);
            playerBottom11.add(downArrow1);
            playerBottom11.add(downArrow2);
            playerBottom11.add(downArrow3);
            playerBottom11.add(downArrow4);           
            playerBottom11.add(downArrow5);
            rightShiftPanel.add(playerRight11);
            playerRight11.add(rightArrow1);
            playerRight11.add(rightArrow2);
            playerRight11.add(rightArrow3);
            playerRight11.add(rightArrow4);           
            playerRight11.add(rightArrow5);             
        }    
        else if(rozmer == 9){                 
            topShiftPanel.add(playerTop9);      
            playerTop9.add(upArrow6);
            playerTop9.add(upArrow7);
            playerTop9.add(upArrow8);
            playerTop9.add(upArrow9);  
            bottomShiftPanel.add(playerBottom9);
            playerBottom9.add(downArrow6);
            playerBottom9.add(downArrow7);
            playerBottom9.add(downArrow9);
            playerBottom9.add(downArrow8);  
            rightShiftPanel.add(playerRight9);
            playerRight9.add(rightArrow6);
            playerRight9.add(rightArrow7);
            playerRight9.add(rightArrow8);
            playerRight9.add(rightArrow9);  
            leftShiftPanel.add(playerLeft9);
            playerLeft9.add(leftArrow7);
            playerLeft9.add(leftArrow8);
            playerLeft9.add(leftArrow6);
            playerLeft9.add(leftArrow9);
        }
        else if (rozmer == 7){
            leftShiftPanel.add(playerLeft7);
            playerLeft7.add(leftArrow10);
            playerLeft7.add(leftArrow11);
            playerLeft7.add(leftArrow12);
            topShiftPanel.add(playerTop7);      
            playerTop7.add(upArrow10);
            playerTop7.add(upArrow11);
            playerTop7.add(upArrow12);
            rightShiftPanel.add(playerRight7);
            playerRight7.add(rightArrow10);
            playerRight7.add(rightArrow11);
            playerRight7.add(rightArrow12);
            bottomShiftPanel.add(playerBottom7);
            playerBottom7.add(downArrow11);
            playerBottom7.add(downArrow10);
            playerBottom7.add(downArrow12);
        }
        else if (rozmer == 5){
            topShiftPanel.add(playerTop5);      
            playerTop5.add(upArrow13);
            playerTop5.add(upArrow14);
            bottomShiftPanel.add(playerBottom5);
            playerBottom5.add(downArrow13);
            playerBottom5.add(downArrow14);
            rightShiftPanel.add(playerRight5);
            playerRight5.add(rightArrow13);
            playerRight5.add(rightArrow14);
            leftShiftPanel.add(playerLeft5);
            playerLeft5.add(leftArrow13);
            playerLeft5.add(leftArrow14);
        }
        
        topShiftPanel.repaint();
        topShiftPanel.revalidate();
    }
    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        leftPanel = new javax.swing.JPanel();
        aktualniHracLabel = new javax.swing.JLabel();
        actualPlayerLabel = new javax.swing.JLabel();
        hledaLabel = new javax.swing.JLabel();
        actualTreasure = new javax.swing.JLabel();
        scoreLabel = new javax.swing.JLabel();
        hrac1Label = new javax.swing.JLabel();
        hrac2Label = new javax.swing.JLabel();
        hrac3Label = new javax.swing.JLabel();
        hrac4Label = new javax.swing.JLabel();
        player1Score = new javax.swing.JLabel();
        player2Score = new javax.swing.JLabel();
        player3Score = new javax.swing.JLabel();
        player4Score = new javax.swing.JLabel();
        showButton = new javax.swing.JButton();
        actualPlayerNumberLabel = new javax.swing.JLabel();
        saveButton = new javax.swing.JButton();
        loadButton = new javax.swing.JButton();
        scoreLabel1 = new javax.swing.JLabel();
        scoreLabel2 = new javax.swing.JLabel();
        middlePanel = new javax.swing.JPanel();
        gameWindowPanel = new javax.swing.JPanel();
        topShiftPanel = new javax.swing.JPanel();
        playerTop11 = new javax.swing.JPanel();
        upArrow4 = new javax.swing.JButton();
        upArrow5 = new javax.swing.JButton();
        upArrow1 = new javax.swing.JButton();
        upArrow2 = new javax.swing.JButton();
        upArrow3 = new javax.swing.JButton();
        playerTop9 = new javax.swing.JPanel();
        upArrow6 = new javax.swing.JButton();
        upArrow7 = new javax.swing.JButton();
        upArrow8 = new javax.swing.JButton();
        upArrow9 = new javax.swing.JButton();
        playerTop7 = new javax.swing.JPanel();
        upArrow12 = new javax.swing.JButton();
        upArrow11 = new javax.swing.JButton();
        upArrow10 = new javax.swing.JButton();
        playerTop5 = new javax.swing.JPanel();
        upArrow13 = new javax.swing.JButton();
        upArrow14 = new javax.swing.JButton();
        bottomShiftPanel = new javax.swing.JPanel();
        playerBottom11 = new javax.swing.JPanel();
        downArrow3 = new javax.swing.JButton();
        downArrow5 = new javax.swing.JButton();
        downArrow4 = new javax.swing.JButton();
        downArrow2 = new javax.swing.JButton();
        downArrow1 = new javax.swing.JButton();
        playerBottom9 = new javax.swing.JPanel();
        downArrow6 = new javax.swing.JButton();
        downArrow7 = new javax.swing.JButton();
        downArrow9 = new javax.swing.JButton();
        downArrow8 = new javax.swing.JButton();
        playerBottom7 = new javax.swing.JPanel();
        downArrow10 = new javax.swing.JButton();
        downArrow11 = new javax.swing.JButton();
        downArrow12 = new javax.swing.JButton();
        playerBottom5 = new javax.swing.JPanel();
        downArrow13 = new javax.swing.JButton();
        downArrow14 = new javax.swing.JButton();
        rightShiftPanel = new javax.swing.JPanel();
        playerRight11 = new javax.swing.JPanel();
        rightArrow1 = new javax.swing.JButton();
        rightArrow3 = new javax.swing.JButton();
        rightArrow5 = new javax.swing.JButton();
        rightArrow2 = new javax.swing.JButton();
        rightArrow4 = new javax.swing.JButton();
        playerRight9 = new javax.swing.JPanel();
        rightArrow6 = new javax.swing.JButton();
        rightArrow7 = new javax.swing.JButton();
        rightArrow8 = new javax.swing.JButton();
        rightArrow9 = new javax.swing.JButton();
        playerRight7 = new javax.swing.JPanel();
        rightArrow10 = new javax.swing.JButton();
        rightArrow11 = new javax.swing.JButton();
        rightArrow12 = new javax.swing.JButton();
        playerRight5 = new javax.swing.JPanel();
        rightArrow13 = new javax.swing.JButton();
        rightArrow14 = new javax.swing.JButton();
        rightPanel = new javax.swing.JPanel();
        jPanel1 = new javax.swing.JPanel();
        jButton2 = new javax.swing.JButton();
        jButton6 = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();
        endTurnButton = new javax.swing.JButton();
        jPanel3 = new javax.swing.JPanel();
        undoButton = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jPanel4 = new javax.swing.JPanel();
        jPanel5 = new javax.swing.JPanel();
        rotateButton = new javax.swing.JButton();
        downButton = new javax.swing.JButton();
        rightButton = new javax.swing.JButton();
        leftButton = new javax.swing.JButton();
        upButton = new javax.swing.JButton();
        leftShiftPanel = new javax.swing.JPanel();
        playerLeft11 = new javax.swing.JPanel();
        leftArrow1 = new javax.swing.JButton();
        leftArrow2 = new javax.swing.JButton();
        leftArrow3 = new javax.swing.JButton();
        leftArrow4 = new javax.swing.JButton();
        leftArrow5 = new javax.swing.JButton();
        playerLeft9 = new javax.swing.JPanel();
        leftArrow7 = new javax.swing.JButton();
        leftArrow8 = new javax.swing.JButton();
        leftArrow6 = new javax.swing.JButton();
        leftArrow9 = new javax.swing.JButton();
        playerLeft7 = new javax.swing.JPanel();
        leftArrow10 = new javax.swing.JButton();
        leftArrow11 = new javax.swing.JButton();
        leftArrow12 = new javax.swing.JButton();
        playerLeft5 = new javax.swing.JPanel();
        leftArrow13 = new javax.swing.JButton();
        leftArrow14 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Labyrinth - The Game");
        setMaximumSize(new java.awt.Dimension(1060, 750));
        setMinimumSize(new java.awt.Dimension(1060, 750));
        setPreferredSize(new java.awt.Dimension(1060, 750));
        setSize(new java.awt.Dimension(1060, 750));
        addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                KeyHandlerGame(evt);
            }
        });

        leftPanel.setBackground(new java.awt.Color(210, 210, 210));

        aktualniHracLabel.setFont(new java.awt.Font("Times New Roman", 0, 20)); // NOI18N
        aktualniHracLabel.setText("Aktualni hrac");

        actualPlayerLabel.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/heroes/heroes1.png"))); // NOI18N

        hledaLabel.setFont(new java.awt.Font("Times New Roman", 0, 20)); // NOI18N
        hledaLabel.setText("Hleda poklad");

        scoreLabel.setFont(new java.awt.Font("Times New Roman", 0, 20)); // NOI18N
        scoreLabel.setText("SCORE");

        hrac1Label.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        hrac1Label.setText("Hrac 1:");

        hrac2Label.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        hrac2Label.setText("Hrac 2:");

        hrac3Label.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        hrac3Label.setText("Hrac 3:");

        hrac4Label.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        hrac4Label.setText("Hrac 4:");

        player1Score.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        player1Score.setText("0");

        player2Score.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        player2Score.setText("0");

        player3Score.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        player3Score.setText("0");

        player4Score.setFont(new java.awt.Font("Times New Roman", 0, 15)); // NOI18N
        player4Score.setText("0");

        showButton.setText("Ukaz");
        showButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                showButtonActionPerformed(evt);
            }
        });

        actualPlayerNumberLabel.setFont(new java.awt.Font("Times New Roman", 0, 18)); // NOI18N
        actualPlayerNumberLabel.setText("Hrac 1");

        saveButton.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/saveGameIcon.png"))); // NOI18N
        saveButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                saveButtonActionPerformed(evt);
            }
        });

        loadButton.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/loadGameIcon.png"))); // NOI18N
        loadButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                loadButtonActionPerformed(evt);
            }
        });

        scoreLabel1.setFont(new java.awt.Font("Times New Roman", 0, 20)); // NOI18N
        scoreLabel1.setText("Nahraj hru");

        scoreLabel2.setFont(new java.awt.Font("Times New Roman", 0, 20)); // NOI18N
        scoreLabel2.setText("Uloz hru");

        javax.swing.GroupLayout leftPanelLayout = new javax.swing.GroupLayout(leftPanel);
        leftPanel.setLayout(leftPanelLayout);
        leftPanelLayout.setHorizontalGroup(
            leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(leftPanelLayout.createSequentialGroup()
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(leftPanelLayout.createSequentialGroup()
                        .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addGap(44, 44, 44)
                                .addComponent(actualPlayerLabel))
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(scoreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 107, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(aktualniHracLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 107, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addGap(35, 35, 35)
                                .addComponent(actualPlayerNumberLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 57, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, leftPanelLayout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(hledaLabel)
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addGap(8, 8, 8)
                                .addComponent(showButton, javax.swing.GroupLayout.PREFERRED_SIZE, 87, javax.swing.GroupLayout.PREFERRED_SIZE)))))
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, leftPanelLayout.createSequentialGroup()
                .addGap(0, 0, Short.MAX_VALUE)
                .addComponent(actualTreasure, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(44, 44, 44))
            .addGroup(leftPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addGroup(leftPanelLayout.createSequentialGroup()
                        .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addComponent(hrac1Label, javax.swing.GroupLayout.PREFERRED_SIZE, 52, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(player1Score, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addComponent(hrac2Label, javax.swing.GroupLayout.PREFERRED_SIZE, 52, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(player2Score, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(leftPanelLayout.createSequentialGroup()
                                .addComponent(hrac4Label, javax.swing.GroupLayout.PREFERRED_SIZE, 52, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(player4Score, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addComponent(scoreLabel2)
                            .addComponent(saveButton, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(scoreLabel1)
                            .addComponent(loadButton, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(leftPanelLayout.createSequentialGroup()
                        .addComponent(hrac3Label, javax.swing.GroupLayout.PREFERRED_SIZE, 52, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(player3Score, javax.swing.GroupLayout.PREFERRED_SIZE, 18, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(37, 37, 37))))
        );
        leftPanelLayout.setVerticalGroup(
            leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(leftPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(aktualniHracLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(actualPlayerLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(actualPlayerNumberLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 19, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(hledaLabel)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(actualTreasure, javax.swing.GroupLayout.PREFERRED_SIZE, 44, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(showButton, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(scoreLabel, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(hrac1Label, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(player1Score, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(hrac2Label, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(player2Score, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(hrac3Label, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(player3Score, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(leftPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(player4Score, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(hrac4Label, javax.swing.GroupLayout.PREFERRED_SIZE, 26, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(scoreLabel2)
                .addGap(10, 10, 10)
                .addComponent(saveButton, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(scoreLabel1)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(loadButton, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(67, 67, 67))
        );

        gameWindowPanel.setLayout(new java.awt.BorderLayout());

        topShiftPanel.setLayout(new java.awt.CardLayout());

        upArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow4ActionPerformed(evt);
            }
        });

        upArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow5ActionPerformed(evt);
            }
        });

        upArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow1ActionPerformed(evt);
            }
        });

        upArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow2ActionPerformed(evt);
            }
        });

        upArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow3ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerTop11Layout = new javax.swing.GroupLayout(playerTop11);
        playerTop11.setLayout(playerTop11Layout);
        playerTop11Layout.setHorizontalGroup(
            playerTop11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerTop11Layout.createSequentialGroup()
                .addGap(70, 70, 70)
                .addComponent(upArrow1, javax.swing.GroupLayout.PREFERRED_SIZE, 48, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(71, 71, 71)
                .addComponent(upArrow2, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(57, 57, 57)
                .addComponent(upArrow3, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(61, 61, 61)
                .addComponent(upArrow4, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(61, 61, 61)
                .addComponent(upArrow5, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(70, Short.MAX_VALUE))
        );
        playerTop11Layout.setVerticalGroup(
            playerTop11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop11Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(playerTop11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(upArrow4, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(upArrow3, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(upArrow2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(upArrow5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(upArrow1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
        );

        topShiftPanel.add(playerTop11, "card5");

        upArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow6ActionPerformed(evt);
            }
        });

        upArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow7ActionPerformed(evt);
            }
        });

        upArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow8ActionPerformed(evt);
            }
        });

        upArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow9ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerTop9Layout = new javax.swing.GroupLayout(playerTop9);
        playerTop9.setLayout(playerTop9Layout);
        playerTop9Layout.setHorizontalGroup(
            playerTop9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerTop9Layout.createSequentialGroup()
                .addContainerGap(96, Short.MAX_VALUE)
                .addComponent(upArrow6)
                .addGap(41, 41, 41)
                .addComponent(upArrow7)
                .addGap(33, 33, 33)
                .addComponent(upArrow8)
                .addGap(37, 37, 37)
                .addComponent(upArrow9)
                .addGap(115, 115, 115))
        );
        playerTop9Layout.setVerticalGroup(
            playerTop9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop9Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(playerTop9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(upArrow6, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow7, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow8, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow9, javax.swing.GroupLayout.Alignment.TRAILING)))
        );

        topShiftPanel.add(playerTop9, "card4");

        upArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow12ActionPerformed(evt);
            }
        });

        upArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow11ActionPerformed(evt);
            }
        });

        upArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow10ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerTop7Layout = new javax.swing.GroupLayout(playerTop7);
        playerTop7.setLayout(playerTop7Layout);
        playerTop7Layout.setHorizontalGroup(
            playerTop7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop7Layout.createSequentialGroup()
                .addContainerGap(162, Short.MAX_VALUE)
                .addComponent(upArrow10)
                .addGap(33, 33, 33)
                .addComponent(upArrow11)
                .addGap(37, 37, 37)
                .addComponent(upArrow12)
                .addGap(169, 169, 169))
        );
        playerTop7Layout.setVerticalGroup(
            playerTop7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop7Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(playerTop7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(upArrow10, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow12, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow11, javax.swing.GroupLayout.Alignment.TRAILING)))
        );

        topShiftPanel.add(playerTop7, "card3");

        upArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow13ActionPerformed(evt);
            }
        });

        upArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrow.png"))); // NOI18N
        upArrow14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upArrow14ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerTop5Layout = new javax.swing.GroupLayout(playerTop5);
        playerTop5.setLayout(playerTop5Layout);
        playerTop5Layout.setHorizontalGroup(
            playerTop5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop5Layout.createSequentialGroup()
                .addGap(218, 218, 218)
                .addComponent(upArrow13)
                .addGap(42, 42, 42)
                .addComponent(upArrow14)
                .addContainerGap(220, Short.MAX_VALUE))
        );
        playerTop5Layout.setVerticalGroup(
            playerTop5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerTop5Layout.createSequentialGroup()
                .addGap(0, 16, Short.MAX_VALUE)
                .addGroup(playerTop5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(upArrow13, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(upArrow14, javax.swing.GroupLayout.Alignment.TRAILING)))
        );

        topShiftPanel.add(playerTop5, "card2");

        bottomShiftPanel.setLayout(new java.awt.CardLayout());

        downArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow3ActionPerformed(evt);
            }
        });

        downArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow5ActionPerformed(evt);
            }
        });

        downArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow4ActionPerformed(evt);
            }
        });

        downArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow2ActionPerformed(evt);
            }
        });

        downArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow1ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerBottom11Layout = new javax.swing.GroupLayout(playerBottom11);
        playerBottom11.setLayout(playerBottom11Layout);
        playerBottom11Layout.setHorizontalGroup(
            playerBottom11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerBottom11Layout.createSequentialGroup()
                .addGap(62, 62, 62)
                .addComponent(downArrow1, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 72, Short.MAX_VALUE)
                .addComponent(downArrow2, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(58, 58, 58)
                .addComponent(downArrow3, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(63, 63, 63)
                .addComponent(downArrow4, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(66, 66, 66)
                .addComponent(downArrow5, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(67, 67, 67))
        );
        playerBottom11Layout.setVerticalGroup(
            playerBottom11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerBottom11Layout.createSequentialGroup()
                .addGroup(playerBottom11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(downArrow3)
                    .addComponent(downArrow5)
                    .addComponent(downArrow4)
                    .addComponent(downArrow2)
                    .addComponent(downArrow1))
                .addGap(0, 64, Short.MAX_VALUE))
        );

        bottomShiftPanel.add(playerBottom11, "card4");

        downArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow6ActionPerformed(evt);
            }
        });

        downArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow7ActionPerformed(evt);
            }
        });

        downArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow9ActionPerformed(evt);
            }
        });

        downArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow8ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerBottom9Layout = new javax.swing.GroupLayout(playerBottom9);
        playerBottom9.setLayout(playerBottom9Layout);
        playerBottom9Layout.setHorizontalGroup(
            playerBottom9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerBottom9Layout.createSequentialGroup()
                .addContainerGap(124, Short.MAX_VALUE)
                .addComponent(downArrow6, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(63, 63, 63)
                .addComponent(downArrow7, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(58, 58, 58)
                .addComponent(downArrow8, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(67, 67, 67)
                .addComponent(downArrow9, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(126, 126, 126))
        );
        playerBottom9Layout.setVerticalGroup(
            playerBottom9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerBottom9Layout.createSequentialGroup()
                .addGroup(playerBottom9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(downArrow9, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(downArrow6, javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(downArrow8, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(downArrow7, javax.swing.GroupLayout.Alignment.LEADING))
                .addGap(0, 63, Short.MAX_VALUE))
        );

        bottomShiftPanel.add(playerBottom9, "card4");

        playerBottom7.setMinimumSize(new java.awt.Dimension(638, 87));
        playerBottom7.setPreferredSize(new java.awt.Dimension(638, 87));

        downArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow10ActionPerformed(evt);
            }
        });

        downArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow11ActionPerformed(evt);
            }
        });

        downArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow12ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerBottom7Layout = new javax.swing.GroupLayout(playerBottom7);
        playerBottom7.setLayout(playerBottom7Layout);
        playerBottom7Layout.setHorizontalGroup(
            playerBottom7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerBottom7Layout.createSequentialGroup()
                .addGap(183, 183, 183)
                .addComponent(downArrow10, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(57, 57, 57)
                .addComponent(downArrow11, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(60, 60, 60)
                .addComponent(downArrow12, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(188, Short.MAX_VALUE))
        );
        playerBottom7Layout.setVerticalGroup(
            playerBottom7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerBottom7Layout.createSequentialGroup()
                .addGroup(playerBottom7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(downArrow12)
                    .addComponent(downArrow10)
                    .addComponent(downArrow11))
                .addGap(0, 64, Short.MAX_VALUE))
        );

        bottomShiftPanel.add(playerBottom7, "card4");

        downArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow13ActionPerformed(evt);
            }
        });

        downArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrow.png"))); // NOI18N
        downArrow14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downArrow14ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerBottom5Layout = new javax.swing.GroupLayout(playerBottom5);
        playerBottom5.setLayout(playerBottom5Layout);
        playerBottom5Layout.setHorizontalGroup(
            playerBottom5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, playerBottom5Layout.createSequentialGroup()
                .addContainerGap(227, Short.MAX_VALUE)
                .addComponent(downArrow13, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(73, 73, 73)
                .addComponent(downArrow14, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(238, 238, 238))
        );
        playerBottom5Layout.setVerticalGroup(
            playerBottom5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerBottom5Layout.createSequentialGroup()
                .addGroup(playerBottom5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(downArrow14)
                    .addComponent(downArrow13))
                .addGap(0, 64, Short.MAX_VALUE))
        );

        bottomShiftPanel.add(playerBottom5, "card4");

        rightShiftPanel.setLayout(new java.awt.CardLayout());

        rightArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow1ActionPerformed(evt);
            }
        });

        rightArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow3ActionPerformed(evt);
            }
        });

        rightArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow5ActionPerformed(evt);
            }
        });

        rightArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow2ActionPerformed(evt);
            }
        });

        rightArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow4ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerRight11Layout = new javax.swing.GroupLayout(playerRight11);
        playerRight11.setLayout(playerRight11Layout);
        playerRight11Layout.setHorizontalGroup(
            playerRight11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight11Layout.createSequentialGroup()
                .addGroup(playerRight11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(rightArrow1, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow2, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow3, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow4, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow5, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(0, 45, Short.MAX_VALUE))
        );
        playerRight11Layout.setVerticalGroup(
            playerRight11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight11Layout.createSequentialGroup()
                .addContainerGap(100, Short.MAX_VALUE)
                .addComponent(rightArrow1, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(54, 54, 54)
                .addComponent(rightArrow2, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(53, 53, 53)
                .addComponent(rightArrow3, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(54, 54, 54)
                .addComponent(rightArrow4, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(49, 49, 49)
                .addComponent(rightArrow5, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(148, 148, 148))
        );

        rightShiftPanel.add(playerRight11, "card5");

        playerRight9.setMinimumSize(new java.awt.Dimension(60, 750));
        playerRight9.setPreferredSize(new java.awt.Dimension(60, 750));

        rightArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow6ActionPerformed(evt);
            }
        });

        rightArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow7ActionPerformed(evt);
            }
        });

        rightArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow8ActionPerformed(evt);
            }
        });

        rightArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow9ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerRight9Layout = new javax.swing.GroupLayout(playerRight9);
        playerRight9.setLayout(playerRight9Layout);
        playerRight9Layout.setHorizontalGroup(
            playerRight9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight9Layout.createSequentialGroup()
                .addGroup(playerRight9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(rightArrow9, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow8, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow7, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow6, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(0, 45, Short.MAX_VALUE))
        );
        playerRight9Layout.setVerticalGroup(
            playerRight9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight9Layout.createSequentialGroup()
                .addGap(148, 148, 148)
                .addComponent(rightArrow6, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(53, 53, 53)
                .addComponent(rightArrow7, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(54, 54, 54)
                .addComponent(rightArrow8, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(56, 56, 56)
                .addComponent(rightArrow9, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(199, Short.MAX_VALUE))
        );

        rightShiftPanel.add(playerRight9, "card5");

        playerRight7.setMinimumSize(new java.awt.Dimension(60, 750));
        playerRight7.setPreferredSize(new java.awt.Dimension(60, 750));

        rightArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow10ActionPerformed(evt);
            }
        });

        rightArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow11ActionPerformed(evt);
            }
        });

        rightArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow12ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerRight7Layout = new javax.swing.GroupLayout(playerRight7);
        playerRight7.setLayout(playerRight7Layout);
        playerRight7Layout.setHorizontalGroup(
            playerRight7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight7Layout.createSequentialGroup()
                .addGroup(playerRight7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(rightArrow11, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow10, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow12, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(0, 45, Short.MAX_VALUE))
        );
        playerRight7Layout.setVerticalGroup(
            playerRight7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight7Layout.createSequentialGroup()
                .addGap(204, 204, 204)
                .addComponent(rightArrow10, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(53, 53, 53)
                .addComponent(rightArrow11, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(54, 54, 54)
                .addComponent(rightArrow12, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(259, Short.MAX_VALUE))
        );

        rightShiftPanel.add(playerRight7, "card5");

        rightArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow13ActionPerformed(evt);
            }
        });

        rightArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrow.png"))); // NOI18N
        rightArrow14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightArrow14ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerRight5Layout = new javax.swing.GroupLayout(playerRight5);
        playerRight5.setLayout(playerRight5Layout);
        playerRight5Layout.setHorizontalGroup(
            playerRight5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight5Layout.createSequentialGroup()
                .addGroup(playerRight5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(rightArrow13, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(rightArrow14, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(0, 45, Short.MAX_VALUE))
        );
        playerRight5Layout.setVerticalGroup(
            playerRight5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerRight5Layout.createSequentialGroup()
                .addGap(265, 265, 265)
                .addComponent(rightArrow13, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(46, 46, 46)
                .addComponent(rightArrow14, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(319, Short.MAX_VALUE))
        );

        rightShiftPanel.add(playerRight5, "card5");

        javax.swing.GroupLayout middlePanelLayout = new javax.swing.GroupLayout(middlePanel);
        middlePanel.setLayout(middlePanelLayout);
        middlePanelLayout.setHorizontalGroup(
            middlePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(middlePanelLayout.createSequentialGroup()
                .addGroup(middlePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(topShiftPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(middlePanelLayout.createSequentialGroup()
                        .addComponent(gameWindowPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 638, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 0, Short.MAX_VALUE))
                    .addComponent(bottomShiftPanel, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(rightShiftPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(1, 1, 1))
        );
        middlePanelLayout.setVerticalGroup(
            middlePanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(middlePanelLayout.createSequentialGroup()
                .addComponent(topShiftPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 32, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gameWindowPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 619, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(bottomShiftPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addComponent(rightShiftPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        rightPanel.setBackground(new java.awt.Color(210, 210, 210));
        rightPanel.setForeground(new java.awt.Color(255, 160, 0));

        jPanel1.setBackground(new java.awt.Color(210, 210, 210));

        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jButton6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton6ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jButton6, javax.swing.GroupLayout.PREFERRED_SIZE, 67, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 73, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(29, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, 49, Short.MAX_VALUE)
                    .addComponent(jButton6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(56, Short.MAX_VALUE))
        );

        jPanel2.setBackground(new java.awt.Color(210, 210, 210));

        endTurnButton.setText("Konec tahu");
        endTurnButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                endTurnButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(18, 18, 18)
                .addComponent(endTurnButton, javax.swing.GroupLayout.PREFERRED_SIZE, 127, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(33, 33, 33)
                .addComponent(endTurnButton, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(42, Short.MAX_VALUE))
        );

        jPanel3.setBackground(new java.awt.Color(210, 210, 210));

        undoButton.setText("Undo");
        undoButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                undoButtonActionPerformed(evt);
            }
        });

        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/herniPanel.png"))); // NOI18N

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(undoButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(18, 18, 18)
                .addComponent(undoButton, javax.swing.GroupLayout.PREFERRED_SIZE, 55, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        jPanel4.setBackground(new java.awt.Color(210, 210, 210));
        jPanel4.setLayout(new java.awt.BorderLayout());

        jPanel5.setBackground(new java.awt.Color(210, 210, 210));

        rotateButton.setText("Rotate");
        rotateButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rotateButtonActionPerformed(evt);
            }
        });

        downButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                downButtonActionPerformed(evt);
            }
        });

        rightButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                rightButtonActionPerformed(evt);
            }
        });

        leftButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftButtonActionPerformed(evt);
            }
        });

        upButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                upButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addGap(20, 20, 20)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(rotateButton, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(leftButton, javax.swing.GroupLayout.PREFERRED_SIZE, 37, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(9, 9, 9)
                        .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(downButton, javax.swing.GroupLayout.PREFERRED_SIZE, 37, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(upButton, javax.swing.GroupLayout.PREFERRED_SIZE, 37, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(rightButton, javax.swing.GroupLayout.PREFERRED_SIZE, 37, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(39, Short.MAX_VALUE))
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel5Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(upButton, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(rightButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(downButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(rotateButton, javax.swing.GroupLayout.PREFERRED_SIZE, 33, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(26, 26, 26))
                    .addGroup(jPanel5Layout.createSequentialGroup()
                        .addComponent(leftButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
        );

        javax.swing.GroupLayout rightPanelLayout = new javax.swing.GroupLayout(rightPanel);
        rightPanel.setLayout(rightPanelLayout);
        rightPanelLayout.setHorizontalGroup(
            rightPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(jPanel2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(jPanel4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(rightPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        rightPanelLayout.setVerticalGroup(
            rightPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(rightPanelLayout.createSequentialGroup()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, 94, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        leftShiftPanel.setLayout(new java.awt.CardLayout());

        leftArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow1ActionPerformed(evt);
            }
        });

        leftArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow2ActionPerformed(evt);
            }
        });

        leftArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow3ActionPerformed(evt);
            }
        });

        leftArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow4ActionPerformed(evt);
            }
        });

        leftArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow5ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerLeft11Layout = new javax.swing.GroupLayout(playerLeft11);
        playerLeft11.setLayout(playerLeft11Layout);
        playerLeft11Layout.setHorizontalGroup(
            playerLeft11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft11Layout.createSequentialGroup()
                .addGap(0, 16, Short.MAX_VALUE)
                .addGroup(playerLeft11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(leftArrow1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow3, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow4, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow5, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        playerLeft11Layout.setVerticalGroup(
            playerLeft11Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft11Layout.createSequentialGroup()
                .addGap(95, 95, 95)
                .addComponent(leftArrow1, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(57, 57, 57)
                .addComponent(leftArrow2, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(51, 51, 51)
                .addComponent(leftArrow3, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(51, 51, 51)
                .addComponent(leftArrow4, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(51, 51, 51)
                .addComponent(leftArrow5, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(145, Short.MAX_VALUE))
        );

        leftShiftPanel.add(playerLeft11, "card2");

        leftArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow7.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow7ActionPerformed(evt);
            }
        });

        leftArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow8.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow8ActionPerformed(evt);
            }
        });

        leftArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow6.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow6ActionPerformed(evt);
            }
        });

        leftArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow9.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow9ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerLeft9Layout = new javax.swing.GroupLayout(playerLeft9);
        playerLeft9.setLayout(playerLeft9Layout);
        playerLeft9Layout.setHorizontalGroup(
            playerLeft9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft9Layout.createSequentialGroup()
                .addGap(0, 16, Short.MAX_VALUE)
                .addGroup(playerLeft9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(leftArrow9, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow8, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow7, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow6, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        playerLeft9Layout.setVerticalGroup(
            playerLeft9Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft9Layout.createSequentialGroup()
                .addGap(153, 153, 153)
                .addComponent(leftArrow6, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(49, 49, 49)
                .addComponent(leftArrow7, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(53, 53, 53)
                .addComponent(leftArrow8, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(56, 56, 56)
                .addComponent(leftArrow9, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(199, Short.MAX_VALUE))
        );

        leftShiftPanel.add(playerLeft9, "card2");

        leftArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow10.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow10ActionPerformed(evt);
            }
        });

        leftArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow11ActionPerformed(evt);
            }
        });

        leftArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow12.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow12ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerLeft7Layout = new javax.swing.GroupLayout(playerLeft7);
        playerLeft7.setLayout(playerLeft7Layout);
        playerLeft7Layout.setHorizontalGroup(
            playerLeft7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft7Layout.createSequentialGroup()
                .addGap(0, 16, Short.MAX_VALUE)
                .addGroup(playerLeft7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(leftArrow10, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow11, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow12, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        playerLeft7Layout.setVerticalGroup(
            playerLeft7Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft7Layout.createSequentialGroup()
                .addGap(210, 210, 210)
                .addComponent(leftArrow10, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(49, 49, 49)
                .addComponent(leftArrow11, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(53, 53, 53)
                .addComponent(leftArrow12, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(258, Short.MAX_VALUE))
        );

        leftShiftPanel.add(playerLeft7, "card2");

        leftArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow13ActionPerformed(evt);
            }
        });

        leftArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrow.png"))); // NOI18N
        leftArrow14.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                leftArrow14ActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout playerLeft5Layout = new javax.swing.GroupLayout(playerLeft5);
        playerLeft5.setLayout(playerLeft5Layout);
        playerLeft5Layout.setHorizontalGroup(
            playerLeft5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft5Layout.createSequentialGroup()
                .addGap(0, 18, Short.MAX_VALUE)
                .addGroup(playerLeft5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(leftArrow13, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(leftArrow14, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );
        playerLeft5Layout.setVerticalGroup(
            playerLeft5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(playerLeft5Layout.createSequentialGroup()
                .addGap(259, 259, 259)
                .addComponent(leftArrow13, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(59, 59, 59)
                .addComponent(leftArrow14, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(312, Short.MAX_VALUE))
        );

        leftShiftPanel.add(playerLeft5, "card2");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(leftPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(leftShiftPanel, javax.swing.GroupLayout.PREFERRED_SIZE, 31, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(middlePanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(rightPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(leftPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(middlePanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(rightPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(leftShiftPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
     * Metoda starající se tlačítko rotace pro volnou kartu.
     * @param evt Actio event evt.
     */
    private void rotateButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rotateButtonActionPerformed
        // TODO add your handling code here:
        MazeCard freeCard = game.getFreeCard();
        freeCard.turnRight();
        GameComponent GC2 = new GameComponent(rozmer, game, 2);
        jPanel4.removeAll();
        jPanel4.add(GC2);
        jPanel4.repaint();
        jPanel4.revalidate();
        this.requestFocusInWindow();
    }//GEN-LAST:event_rotateButtonActionPerformed

    /**
     * Hlavní key handler starající se o stisknuté klávesy a vytvářející správnou reakci na ně.
     * Také překresluje hrací plochu při každém volání (jelikož po stisknutí klávesy se v 90% něco děje).
     * @param evt Key event evt.
     */
    private void KeyHandlerGame(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_KeyHandlerGame
        // TODO add your handling code here:
        if(evt.getKeyCode() == KeyEvent.VK_SPACE)
        {
            rotateButton.doClick();
            repaintAll();
        }
        else if(evt.getKeyCode() == KeyEvent.VK_UP){
            upButton.doClick();
        }
        else if(evt.getKeyCode() == KeyEvent.VK_DOWN){
            downButton.doClick();            
        }
        else if(evt.getKeyCode() == KeyEvent.VK_LEFT){
            leftButton.doClick(); 
        }
        else if(evt.getKeyCode() == KeyEvent.VK_RIGHT){
            rightButton.doClick(); 
        }
        else if(evt.getKeyCode() == KeyEvent.VK_ENTER){
            this.thisTurnShift = false;
            endTurnButton.doClick();
            repaintAll();
            
        }
        else if(evt.getKeyCode() == KeyEvent.VK_SHIFT)
        {
            showButton.doClick();            
        }
        else if(evt.getKeyCode() == KeyEvent.VK_ESCAPE){
            menu.setVisible(true);
            menu.getBackToMenu();
            this.setVisible(false);
        }
        this.requestFocusInWindow();
        
    }//GEN-LAST:event_KeyHandlerGame
    /**
     * Funkce, která překreslí celé hrací pole.
     */
    private void repaintAll(){
        /* po kazdem pohybu vykresli hraci plochu znovu */
        gameWindowPanel.removeAll();
        GameComponent newGC = new GameComponent(rozmer, game, 1);
        gameWindowPanel.add(newGC);        
        gameWindowPanel.repaint();
        gameWindowPanel.revalidate();
        
        /* actualize score */
        Integer [] pole = game.getPlayersScore();
        player1Score.setText(Integer.toString(pole[0]));
        player2Score.setText(Integer.toString(pole[1]));
        if(numberOfPlayers == 4){
            player3Score.setText(Integer.toString(pole[2]));
            player4Score.setText(Integer.toString(pole[3]));
        }
        else if(numberOfPlayers == 3){
            player3Score.setText(Integer.toString(pole[2]));
        }
        /* vzdu znovu nastav aktualni treasure --> zmena vidna ihned po sebrani treasure */
        setActualTreasure();
        
        /* check if there is a winner */
        if(game.winner() != -1){
            if(numberOfPlayers == 4){
                JOptionPane.showMessageDialog(null, "Hrac"+game.winner()+" vyhrava hru!\n\nHrac1 score: "+Integer.toString(pole[0])+"\nHrac2 score: "+Integer.toString(pole[1])+"\nHrac3 score: "+Integer.toString(pole[2])+"\nHrac4 score: "+Integer.toString(pole[3]));
            }
            else if(numberOfPlayers == 2){
                JOptionPane.showMessageDialog(null, "Hrac"+game.winner()+" vyhrava hru!\n\nHrac1 score: "+Integer.toString(pole[0])+"\nHrac2 score: "+Integer.toString(pole[1]));
            }
            
            System.exit(0);
        }
    }
    
    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        // TODO add your handling code here:
        menu.setVisible(true);
        menu.getBackToMenu();
        this.setVisible(false);
    }//GEN-LAST:event_jButton2ActionPerformed

    private void jButton6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton6ActionPerformed
        // TODO add your handling code here:
        new HelpWindow().setVisible(true);
        this.requestFocusInWindow();
    }//GEN-LAST:event_jButton6ActionPerformed

    private void upArrow5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow5ActionPerformed
        game.shiftCard(1, 10);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }
        this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow5ActionPerformed

    private void rightArrow2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow2ActionPerformed
        game.shiftCard(4, 11);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
        this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow2ActionPerformed

    private void upArrow4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow4ActionPerformed
        game.shiftCard(1, 8);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }         
         this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow4ActionPerformed

    private void upArrow1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow1ActionPerformed
        game.shiftCard(1, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow1ActionPerformed

    private void upArrow2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow2ActionPerformed
        game.shiftCard(1, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow2ActionPerformed

    private void upArrow3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow3ActionPerformed
        game.shiftCard(1, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow3ActionPerformed

    private void downArrow3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow3ActionPerformed
        game.shiftCard(11, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow3ActionPerformed

    private void downArrow5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow5ActionPerformed
        game.shiftCard(11, 10);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow5ActionPerformed

    private void downArrow4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow4ActionPerformed
        game.shiftCard(11, 8);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow4ActionPerformed

    private void downArrow2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow2ActionPerformed
        game.shiftCard(11, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow2ActionPerformed

    private void downArrow1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow1ActionPerformed
        game.shiftCard(11, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow1ActionPerformed

    private void rightArrow5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow5ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(10, 11);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
        this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow5ActionPerformed

    private void rightArrow4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow4ActionPerformed
        game.shiftCard(8, 11);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow4ActionPerformed

    private void rightArrow3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow3ActionPerformed
        game.shiftCard(6, 11);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow3ActionPerformed

    private void rightArrow1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow1ActionPerformed
        game.shiftCard(2, 11);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow1ActionPerformed

    private void leftArrow1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow1ActionPerformed
        game.shiftCard(2, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow1ActionPerformed

    private void leftArrow2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow2ActionPerformed
        game.shiftCard(4, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow2.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow2ActionPerformed

    private void leftArrow3ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow3ActionPerformed
        game.shiftCard(6, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow3ActionPerformed

    private void leftArrow4ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow4ActionPerformed
        game.shiftCard(8, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow4.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow4ActionPerformed

    private void leftArrow5ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow5ActionPerformed
        game.shiftCard(10, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow5ActionPerformed
    /**
     * Funkce starající se o skrývání/odhalování právě hledaného pokladu.
     * @param evt Action event evt.
     */
    private void showButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_showButtonActionPerformed
        // TODO add your handling code here:
        if(visible == false){
            actualTreasure.setVisible(true);
            showButton.setText("Schovej");
            visible = true;
        }
        else if(visible == true){
            actualTreasure.setVisible(false);
            showButton.setText("Ukaz");
            visible = false;
        }
        this.requestFocusInWindow();
    }//GEN-LAST:event_showButtonActionPerformed
    /**
     * Funkce starající se o přepnutí nového kola.
     * @param evt Action event evt.
     */
    private void endTurnButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_endTurnButtonActionPerformed
        // TODO add your handling code here:
        game.nextTurn();
        setActualPlayerLabel(); 
        this.requestFocusInWindow();
    }//GEN-LAST:event_endTurnButtonActionPerformed

    private void downArrow6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow6ActionPerformed
        game.shiftCard(9, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow6ActionPerformed

    private void downArrow7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow7ActionPerformed
        game.shiftCard(9, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow7ActionPerformed

    private void downArrow9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow9ActionPerformed
        game.shiftCard(9, 8);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow9ActionPerformed

    private void downArrow8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow8ActionPerformed
        game.shiftCard(9, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow8ActionPerformed

    private void rightArrow6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow6ActionPerformed
        game.shiftCard(2, 9);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }
         this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow6ActionPerformed

    private void rightArrow7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow7ActionPerformed
        game.shiftCard(4, 9);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow7ActionPerformed

    private void rightArrow8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow8ActionPerformed
        game.shiftCard(6, 9);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow8ActionPerformed

    private void rightArrow9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow9ActionPerformed
        game.shiftCard(8, 9);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow9ActionPerformed

    private void leftArrow7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow7ActionPerformed
        game.shiftCard(4, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow7ActionPerformed

    private void leftArrow8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow8ActionPerformed
        game.shiftCard(6, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow8ActionPerformed

    private void leftArrow6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow6ActionPerformed
        game.shiftCard(2, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow6ActionPerformed

    private void leftArrow9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow9ActionPerformed
        game.shiftCard(8, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow9ActionPerformed
    /**
     * Metoda starající se o undo button. Hlavní náplní je správné překreslení hrací plochy.
     * @param evt Action event evt.
     */
    private void undoButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_undoButtonActionPerformed
        // TODO add your handling code here:
        game.undo();
        repaintAfterShift();        
        /* actualize score */
        Integer [] pole = game.getPlayersScore();
        player1Score.setText(Integer.toString(pole[0]));
        player2Score.setText(Integer.toString(pole[1]));
        if(numberOfPlayers == 4){
            player3Score.setText(Integer.toString(pole[2]));
            player4Score.setText(Integer.toString(pole[3]));
        }
        /* vzdu znovu nastav aktualni treasure --> zmena vidna ihned po sebrani treasure */
        setActualTreasure();
        setActualPlayerLabel();
        if (game.useShift == false)
            thisTurnShift = false;
        else
            thisTurnShift = true;
        this.requestFocusInWindow();
    }//GEN-LAST:event_undoButtonActionPerformed

    private void upArrow6ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow6ActionPerformed
        game.shiftCard(1, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow6ActionPerformed

    private void upArrow7ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow7ActionPerformed
        game.shiftCard(1, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow7ActionPerformed

    private void upArrow8ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow8ActionPerformed
        game.shiftCard(1, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow8ActionPerformed

    private void upArrow9ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow9ActionPerformed
        game.shiftCard(1, 8);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow9ActionPerformed

    private void leftArrow10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow10ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(2, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow10ActionPerformed

    private void leftArrow11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow11ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(4, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow11ActionPerformed

    private void leftArrow12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow12ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(6, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow12ActionPerformed

    private void rightArrow10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow10ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(2, 7);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow10ActionPerformed

    private void rightArrow11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow11ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(4, 7);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow11ActionPerformed

    private void rightArrow12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow12ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(6, 7);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow12ActionPerformed

    private void downArrow10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow10ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(7, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow10ActionPerformed

    private void downArrow11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow11ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(7, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow11ActionPerformed

    private void downArrow12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow12ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(7, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow12ActionPerformed

    private void upArrow12ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow12ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(1, 6);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow12ActionPerformed

    private void upArrow10ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow10ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(1, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow10ActionPerformed

    private void upArrow11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow11ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(1, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow11ActionPerformed

    private void upArrow13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow13ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(1, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow13ActionPerformed

    private void upArrow14ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upArrow14ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(1, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            downArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/downArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_upArrow14ActionPerformed

    private void downArrow13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow13ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(5, 2);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow13ActionPerformed

    private void downArrow14ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downArrow14ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(5, 4);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            upArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/upArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_downArrow14ActionPerformed

    private void rightArrow13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow13ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(2, 5);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow13ActionPerformed

    private void rightArrow14ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightArrow14ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(4, 5);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            leftArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/leftArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_rightArrow14ActionPerformed

    private void leftArrow13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow13ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(2, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow13ActionPerformed

    private void leftArrow14ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftArrow14ActionPerformed
        // TODO add your handling code here:
        game.shiftCard(4, 1);
         if(game.useShift == true && thisTurnShift == false){ thisTurnShift = true;
            repaintAfterShift();
            rightArrow14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/shift/rightArrowRed.png")));
        }this.requestFocusInWindow();
    }//GEN-LAST:event_leftArrow14ActionPerformed

    private void saveButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveButtonActionPerformed
        new SaveGame(game, 1, null, null).setVisible(true);
        this.requestFocusInWindow();
    }//GEN-LAST:event_saveButtonActionPerformed

    private void upButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_upButtonActionPerformed
        game.movePlayer(MazeCard.CANGO.UP);
        repaintAll();
        this.requestFocusInWindow();
    }//GEN-LAST:event_upButtonActionPerformed

    private void leftButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_leftButtonActionPerformed
        game.movePlayer(MazeCard.CANGO.LEFT);
        repaintAll();
        this.requestFocusInWindow();
    }//GEN-LAST:event_leftButtonActionPerformed

    private void rightButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_rightButtonActionPerformed
        game.movePlayer(MazeCard.CANGO.RIGHT); 
        repaintAll();
        this.requestFocusInWindow();
    }//GEN-LAST:event_rightButtonActionPerformed

    private void downButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_downButtonActionPerformed
        game.movePlayer(MazeCard.CANGO.DOWN); 
        repaintAll();
        this.requestFocusInWindow();
    }//GEN-LAST:event_downButtonActionPerformed

    /**
     * Funkce, která vytvoří nové "nahrávací" okno.
     * @param evt Action event evt.
     */
    private void loadButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_loadButtonActionPerformed
        new SaveGame(null, 2, this.menu, this).setVisible(true);
    }//GEN-LAST:event_loadButtonActionPerformed
    /**
     * Funkce, která překreslí jen určité části hrací plochy po shiftování.
     * Nepoužívá funkci repaint all kvůli mírné optimalizaci.
     */
    private void repaintAfterShift(){
        gameWindowPanel.removeAll();
        GameComponent newGC = new GameComponent(rozmer, game, 1);
        gameWindowPanel.add(newGC);
        GameComponent newGC2 = new GameComponent(rozmer, game, 2);        
        jPanel4.add(newGC2);
        gameWindowPanel.repaint();
        gameWindowPanel.revalidate();
        jPanel4.repaint();
        jPanel4.revalidate();
        setAllArrowsBlack();
        this.requestFocusInWindow();
    }
    /**
     * Nastaví ikonku aktuálního hráče.
     */
    private void setActualPlayerLabel(){
        BufferedImage image; 
        actualHero = game.getActualPlayer().getID();           
        image = null;
        try {
            //image = ImageIO.read(new File("./lib/images/heroes/heroes"+actualHero+".png"));
            image = ImageIO.read(getClass().getResource("/images/heroes/heroes"+actualHero+".png"));
        } catch (IOException ex){}
        actualPlayerLabel.setIcon(new ImageIcon(image));
        actualPlayerNumberLabel.setText("Hrac "+actualHero);
    }
    /**
     * Nastaví ikonku aktuálního pokladu.
     */
    private void setActualTreasure(){
        BufferedImage image;
        image = null;
        try {
            //image = ImageIO.read(new File("./lib/images/poklady/poklad"+((game.getActualPlayerTreasure())+1)+".png"));
            image = ImageIO.read(getClass().getResource("/images/poklady/poklad"+((game.getActualPlayerTreasure())+1)+".png"));
        } catch (IOException ex){}
        actualTreasure.setIcon(new ImageIcon(image));
    }
    /**
     * Funkce, která se stará o správné vykreslení score.
     */
    private void setScoreBoard(){
        if(numberOfPlayers == 2)
        {
            player3Score.setVisible(false);
            player4Score.setVisible(false);
            hrac3Label.setVisible(false);
            hrac4Label.setVisible(false);
        }
        else if(numberOfPlayers == 3){
            player4Score.setVisible(false);
            hrac4Label.setVisible(false); 
        }
    }
    /**
     * Funkce, která nastaví všechny shiftovací šipky na černou.
     */
    private void setAllArrowsBlack(){
        if(rozmer == 5){
            upArrow13.setIcon(GameWindow.upArrow);
            upArrow14.setIcon(GameWindow.upArrow);
            downArrow13.setIcon(GameWindow.downArrow);
            downArrow14.setIcon(GameWindow.downArrow);
            leftArrow13.setIcon(GameWindow.leftArrow);
            leftArrow14.setIcon(GameWindow.leftArrow);
            rightArrow13.setIcon(GameWindow.rightArrow);
            rightArrow14.setIcon(GameWindow.rightArrow);
        }
        else if(rozmer == 7){
            upArrow12.setIcon(GameWindow.upArrow);
            upArrow11.setIcon(GameWindow.upArrow);
            upArrow10.setIcon(GameWindow.upArrow);
            downArrow12.setIcon(GameWindow.downArrow);
            downArrow11.setIcon(GameWindow.downArrow);
            downArrow10.setIcon(GameWindow.downArrow);
            leftArrow12.setIcon(GameWindow.leftArrow);
            leftArrow11.setIcon(GameWindow.leftArrow);
            leftArrow10.setIcon(GameWindow.leftArrow);
            rightArrow12.setIcon(GameWindow.rightArrow);
            rightArrow11.setIcon(GameWindow.rightArrow);
            rightArrow10.setIcon(GameWindow.rightArrow);
        }
        else if(rozmer == 9){
            upArrow9.setIcon(GameWindow.upArrow);
            upArrow8.setIcon(GameWindow.upArrow);
            upArrow7.setIcon(GameWindow.upArrow);
            upArrow6.setIcon(GameWindow.upArrow);
            downArrow8.setIcon(GameWindow.downArrow);
            downArrow9.setIcon(GameWindow.downArrow);
            downArrow7.setIcon(GameWindow.downArrow);
            downArrow6.setIcon(GameWindow.downArrow);
            leftArrow9.setIcon(GameWindow.leftArrow);
            leftArrow8.setIcon(GameWindow.leftArrow);
            leftArrow7.setIcon(GameWindow.leftArrow);
            leftArrow6.setIcon(GameWindow.leftArrow);
            rightArrow9.setIcon(GameWindow.rightArrow);
            rightArrow8.setIcon(GameWindow.rightArrow);
            rightArrow7.setIcon(GameWindow.rightArrow);
            rightArrow6.setIcon(GameWindow.rightArrow); 
        }
        else if(rozmer == 11){
            upArrow5.setIcon(GameWindow.upArrow);
            upArrow4.setIcon(GameWindow.upArrow);
            upArrow3.setIcon(GameWindow.upArrow);
            upArrow2.setIcon(GameWindow.upArrow);
            upArrow1.setIcon(GameWindow.upArrow);
            downArrow5.setIcon(GameWindow.downArrow);
            downArrow4.setIcon(GameWindow.downArrow);
            downArrow3.setIcon(GameWindow.downArrow);
            downArrow2.setIcon(GameWindow.downArrow);
            downArrow1.setIcon(GameWindow.downArrow);
            leftArrow5.setIcon(GameWindow.leftArrow);
            leftArrow4.setIcon(GameWindow.leftArrow);
            leftArrow3.setIcon(GameWindow.leftArrow);
            leftArrow2.setIcon(GameWindow.leftArrow);
            leftArrow1.setIcon(GameWindow.leftArrow);
            rightArrow5.setIcon(GameWindow.rightArrow);
            rightArrow4.setIcon(GameWindow.rightArrow);
            rightArrow3.setIcon(GameWindow.rightArrow);
            rightArrow2.setIcon(GameWindow.rightArrow);
            rightArrow1.setIcon(GameWindow.rightArrow);
        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel actualPlayerLabel;
    private javax.swing.JLabel actualPlayerNumberLabel;
    private javax.swing.JLabel actualTreasure;
    private javax.swing.JLabel aktualniHracLabel;
    private javax.swing.JPanel bottomShiftPanel;
    private javax.swing.JButton downArrow1;
    private javax.swing.JButton downArrow10;
    private javax.swing.JButton downArrow11;
    private javax.swing.JButton downArrow12;
    private javax.swing.JButton downArrow13;
    private javax.swing.JButton downArrow14;
    private javax.swing.JButton downArrow2;
    private javax.swing.JButton downArrow3;
    private javax.swing.JButton downArrow4;
    private javax.swing.JButton downArrow5;
    private javax.swing.JButton downArrow6;
    private javax.swing.JButton downArrow7;
    private javax.swing.JButton downArrow8;
    private javax.swing.JButton downArrow9;
    private javax.swing.JButton downButton;
    private javax.swing.JButton endTurnButton;
    private javax.swing.JPanel gameWindowPanel;
    private javax.swing.JLabel hledaLabel;
    private javax.swing.JLabel hrac1Label;
    private javax.swing.JLabel hrac2Label;
    private javax.swing.JLabel hrac3Label;
    private javax.swing.JLabel hrac4Label;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton6;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JButton leftArrow1;
    private javax.swing.JButton leftArrow10;
    private javax.swing.JButton leftArrow11;
    private javax.swing.JButton leftArrow12;
    private javax.swing.JButton leftArrow13;
    private javax.swing.JButton leftArrow14;
    private javax.swing.JButton leftArrow2;
    private javax.swing.JButton leftArrow3;
    private javax.swing.JButton leftArrow4;
    private javax.swing.JButton leftArrow5;
    private javax.swing.JButton leftArrow6;
    private javax.swing.JButton leftArrow7;
    private javax.swing.JButton leftArrow8;
    private javax.swing.JButton leftArrow9;
    private javax.swing.JButton leftButton;
    private javax.swing.JPanel leftPanel;
    private javax.swing.JPanel leftShiftPanel;
    private javax.swing.JButton loadButton;
    private javax.swing.JPanel middlePanel;
    private javax.swing.JLabel player1Score;
    private javax.swing.JLabel player2Score;
    private javax.swing.JLabel player3Score;
    private javax.swing.JLabel player4Score;
    private javax.swing.JPanel playerBottom11;
    private javax.swing.JPanel playerBottom5;
    private javax.swing.JPanel playerBottom7;
    private javax.swing.JPanel playerBottom9;
    private javax.swing.JPanel playerLeft11;
    private javax.swing.JPanel playerLeft5;
    private javax.swing.JPanel playerLeft7;
    private javax.swing.JPanel playerLeft9;
    private javax.swing.JPanel playerRight11;
    private javax.swing.JPanel playerRight5;
    private javax.swing.JPanel playerRight7;
    private javax.swing.JPanel playerRight9;
    private javax.swing.JPanel playerTop11;
    private javax.swing.JPanel playerTop5;
    private javax.swing.JPanel playerTop7;
    private javax.swing.JPanel playerTop9;
    private javax.swing.JButton rightArrow1;
    private javax.swing.JButton rightArrow10;
    private javax.swing.JButton rightArrow11;
    private javax.swing.JButton rightArrow12;
    private javax.swing.JButton rightArrow13;
    private javax.swing.JButton rightArrow14;
    private javax.swing.JButton rightArrow2;
    private javax.swing.JButton rightArrow3;
    private javax.swing.JButton rightArrow4;
    private javax.swing.JButton rightArrow5;
    private javax.swing.JButton rightArrow6;
    private javax.swing.JButton rightArrow7;
    private javax.swing.JButton rightArrow8;
    private javax.swing.JButton rightArrow9;
    private javax.swing.JButton rightButton;
    private javax.swing.JPanel rightPanel;
    private javax.swing.JPanel rightShiftPanel;
    private javax.swing.JButton rotateButton;
    private javax.swing.JButton saveButton;
    private javax.swing.JLabel scoreLabel;
    private javax.swing.JLabel scoreLabel1;
    private javax.swing.JLabel scoreLabel2;
    private javax.swing.JButton showButton;
    private javax.swing.JPanel topShiftPanel;
    private javax.swing.JButton undoButton;
    private javax.swing.JButton upArrow1;
    private javax.swing.JButton upArrow10;
    private javax.swing.JButton upArrow11;
    private javax.swing.JButton upArrow12;
    private javax.swing.JButton upArrow13;
    private javax.swing.JButton upArrow14;
    private javax.swing.JButton upArrow2;
    private javax.swing.JButton upArrow3;
    private javax.swing.JButton upArrow4;
    private javax.swing.JButton upArrow5;
    private javax.swing.JButton upArrow6;
    private javax.swing.JButton upArrow7;
    private javax.swing.JButton upArrow8;
    private javax.swing.JButton upArrow9;
    private javax.swing.JButton upButton;
    // End of variables declaration//GEN-END:variables
}
