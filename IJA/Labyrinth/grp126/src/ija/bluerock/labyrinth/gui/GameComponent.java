package ija.bluerock.labyrinth.gui;

import ija.bluerock.labyrinth.game.board.MazeCard;
import ija.bluerock.labyrinth.game.board.MazeField;
import ija.bluerock.labyrinth.game.Game;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import javax.swing.JComponent;

/**
 * Třída implementující vykreslení hracího pole, hráče a pokladů.
 * @author Martin Kačmarčík
 */
public class GameComponent extends JComponent{
    private BufferedImage image;
    private final int rozmer;
    private int offset;
    private final int mode;
    private MazeField field;
    private MazeCard card;
    private final Game game;
    private String typeFile;
    
    /**
     * Konstruktor třídy, který inicializuje třídní proměné (private).
     * @param rozmer    Rozměr hrací plochy.
     * @param game  Třída game, obsahující informace o aktuální hře.
     * @param mode  Mód, který rozlišuje vykreslení plochy a volné karty. Mode = 1 je hrací plocha a Mode == 2 je volná karta.
     */
    public GameComponent(int rozmer, Game game, int mode){
        this.rozmer = rozmer;
        this.game = game;
        this.mode = mode;        
    }
    /**
     * Funkce která vypočítá offset posunutí pro vykreslení hrací plochy.
     * Na základě rozměru je vzroečkem spočítaný offset, o který se vždy posune právě vykreslující políčko.
     * @param rozmer    Rozměr značí rozměr hracího pole.
     * @return Vrací velikost offsetu.
     */
    public int calculateOffset(int rozmer){
        return (640-55*rozmer)/2;
    }
    /**
     * Funkce, která vrací rotaci políčka plus nastavuje proměnou typeFile, pro otevření souboru.
     * @param type  Type obsahuje typ políčka (C, F nebo L).
     * @return Vrací aktuální rotaci.
     */
    private void getRotation(String type){
        if(type.equals("C")){
            typeFile = "lko";
            if(card.canGo(MazeCard.CANGO.UP) && card.canGo(MazeCard.CANGO.RIGHT)){                    
                image = GameWindow.lko1;
            }
            else if(card.canGo(MazeCard.CANGO.DOWN) && card.canGo(MazeCard.CANGO.RIGHT)){
                image = GameWindow.lko2;
            }
            else if(card.canGo(MazeCard.CANGO.DOWN) && card.canGo(MazeCard.CANGO.LEFT)){
                image = GameWindow.lko3;
            }
            else if(card.canGo(MazeCard.CANGO.UP) && card.canGo(MazeCard.CANGO.LEFT)){
                image = GameWindow.lko4;
            }
        }
        if(type.equals("L")){
            typeFile = "rovna";
            if((card.canGo(MazeCard.CANGO.UP) && card.canGo(MazeCard.CANGO.DOWN))){   
                image = GameWindow.rovna1;
            }
            else if(card.canGo(MazeCard.CANGO.LEFT) && card.canGo(MazeCard.CANGO.RIGHT)){
                image = GameWindow.rovna2;
            }
        }
        if(type.equals("F")){
            typeFile = "tecko";
            if(card.canGo(MazeCard.CANGO.UP) && card.canGo(MazeCard.CANGO.RIGHT) && card.canGo(MazeCard.CANGO.DOWN)){
                image = GameWindow.tecko1;
            }
            else if(card.canGo(MazeCard.CANGO.LEFT) && card.canGo(MazeCard.CANGO.DOWN) && card.canGo(MazeCard.CANGO.RIGHT)){
                image = GameWindow.tecko2;
            }
            else if(card.canGo(MazeCard.CANGO.LEFT) && card.canGo(MazeCard.CANGO.DOWN) && card.canGo(MazeCard.CANGO.UP)){
                image = GameWindow.tecko3;
            }
            else if(card.canGo(MazeCard.CANGO.LEFT) && card.canGo(MazeCard.CANGO.RIGHT) && card.canGo(MazeCard.CANGO.UP)){
                image = GameWindow.tecko4;
            }
        }
    }
    /**
     * Nacte aktualni obrazek na zaklade souradnic.
     * @param i X-ová souřadnice.
     * @param j Y-ová souřadnice.
     */
    public void selectImage (int i, int j){        
        
        card = game.getMazeCard(i+1, j+1);
        String type = this.card.getType();
        getRotation(type);        
        
    }
    /**
     * Funkce, která vykresluje hrací plochu a volnou kartu.
     * Používá třídá Graphics. 
     * @param g Parametr, který je třídě předán při volání (od JComponenty, kterou třída rozšiřuje).
     */
    @Override
    public void paintComponent(Graphics g){
        
        image = null;
        offset = calculateOffset(rozmer);
        super.paintComponent(g);
        
        if (mode == 1){
            /* draw the whole field */
            for (int i = 0; i < rozmer; i++){
                for (int j = 0; j < rozmer; j++){                
                    selectImage(i, j);
                    g.drawImage(image, offset+j*55+j*1-5, offset+55*i+i*1-10, this);
                    /* painting player/poklad */
                    if(game.getTreasureCode(i+1, j+1) != -1){                        
                        /*try {  
                            //image = ImageIO.read(getClass().getResource("/images/poklady/poklad"+(game.getTreasureCode(i+1, j+1)+1)+".png"));
                        } catch (IOException ex){}*/
                        image = GameWindow.treasureArray[(game.getTreasureCode(i+1, j+1))];
                        g.drawImage(image, offset+j*55+j*1+10-5, offset+55*i+i*1+5-10, this);
                    }
                    if(game.getPlayerHere(i+1, j+1) != -1){
                        /*try {    
                            String icko = Integer.toString(i);
                            String jcko = Integer.toString(j);
                            System.out.println("Na pozici x:"+icko+" y: "+jcko+" je hrac");
                            //image = ImageIO.read(new File("./lib/images/heroes/heroes"+game.getPlayerHere(i+1, j+1)+".png"));
                            //image = ImageIO.read(getClass().getResource("/images/heroes/heroes"+game.getPlayerHere(i+1, j+1)+".png"));                            
                        } catch (IOException ex){}*/
                        image = GameWindow.playersArray[(game.getPlayerHere(i+1, j+1)-1)];
                        g.drawImage(image, offset+j*55+j*1+10-5, offset+55*i+i*1+5-10, this);
                    }
                    
                }
            }
        }        
        if(mode == 2){
            /* draw free card */
            selectImage(-2, -2);
            g.drawImage(image, 50, 0, this);
            /* treasure in free card */
            if(game.getTreasureCode(-1, -1) != -1){                        
                /*try {                            
                    //image = ImageIO.read(new File("./lib/images/poklady/poklad"+(game.getTreasureCode(-1, -1)+1)+".png")); //classLoader.getResource(fileName).getFile()
                    //image = ImageIO.read(getClass().getResource("/images/poklady/poklad"+(game.getTreasureCode(-1, -1)+1)+".png"));
                    image = treasureArray[(game.getTreasureCode(-1, -1))];
                    
                } catch (IOException ex){}*/
                image = GameWindow.treasureArray[(game.getTreasureCode(-1, -1))];
                g.drawImage(image, 50+10, 0+5, this);
            }
        }
        
    }
    /**
     * Overridnutá metoda, která nastavuje správně velikost hrací plochy.
     * @return Vrací dimenzi se správnou velikostí hrací plochy.
     */
    @Override
    public Dimension getPreferredSize(){
        return new Dimension(700, 700);
    }
}
