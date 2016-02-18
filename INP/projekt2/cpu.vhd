-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Martin Kacmarcik -- xkacma03
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

--FSM logika

type fsm_state is (sidle, snop, 
				   sfetch, sdecode, shalt,
				   splus0, splus1, -- +
				   sminus0, sminus1, -- -				   
				   sdec, -- inkrementace hodnoty ukazatele
				   sinc, -- inkrementace hodnoty ukazatele
				   sload0, sload1, --nacti hodnotu aktualni bunky
				   sprint0, sprint1, --vytiskni hodnotu aktualni bunky
				   swstart0, swstart1, swstart2, swstart3, --while cyklus
				   swend0, swend1, swend2, swend3 --while cyklus
				   );
				   
signal pstate : fsm_state;
signal nstate : fsm_state;

type instruction_list is (halt, nop, 
				          plus, minus, inc, dec,
                          while_start, while_end, 
						  print, load);				   
				   				 
signal instruction_decoded : instruction_list;


--programovy citac
signal pc_reg_inc : std_logic;
signal pc_reg_dec : std_logic;
signal pc_reg_addr : std_logic_vector(11 downto 0);

--registr ptr
signal ptr_reg_inc : std_logic;
signal ptr_reg_dec : std_logic;
signal ptr_reg_addr : std_logic_vector(11 downto 0);

--registr CNT
signal cnt_reg_inc : std_logic;
signal cnt_reg_dec : std_logic;
signal cnt_reg_addr : std_logic_vector(7 downto 0);

--multiplexory
signal mx1_sel : std_logic;
signal mx2_sel : std_logic_vector(1 downto 0);


begin


 
--programovy citac
pc_reg: process (RESET, CLK)
   begin
      if (RESET='1') then
         pc_reg_addr <= (others=>'0');
      elsif (CLK'event) and (CLK='1') then
         if (pc_reg_inc='1') then
            pc_reg_addr <= pc_reg_addr + 1;
         elsif (pc_reg_dec='1') then
            pc_reg_addr <= pc_reg_addr - 1;
         end if;
      end if;
  end process;

--registr ptr
ptr_reg: process (RESET, CLK) 
   begin
      if (RESET='1') then
         ptr_reg_addr <= (others=>'0');
      elsif (CLK'event) and (CLK='1') then
         if (ptr_reg_inc='1') then
            ptr_reg_addr <= ptr_reg_addr + 1;
         elsif (ptr_reg_dec='1') then
            ptr_reg_addr <= ptr_reg_addr - 1;
         end if;
      end if;
  end process;
  
--registr CNT
cnt_reg: process (RESET, CLK)
   begin
      if (RESET='1') then
         cnt_reg_addr <= (others=>'0');
      elsif (CLK'event) and (CLK='1') then
         if (cnt_reg_inc='1') then
            cnt_reg_addr <= cnt_reg_addr + 1;
         elsif (cnt_reg_dec='1') then
            cnt_reg_addr <= cnt_reg_addr - 1;
         end if;
      end if;
  end process;
  
--multiplexor MX1
process(mx1_sel, pc_reg_addr, ptr_reg_addr)
begin
   case mx1_sel is
      when '0' => DATA_ADDR <= '0'&pc_reg_addr; --PC
      when '1' => DATA_ADDR <= '1'&ptr_reg_addr; --PTR
	  when others => DATA_ADDR <= '0'&pc_reg_addr; --PC
   end case;
end process;

--Multiplexor MX2
process(mx2_sel, DATA_RDATA, IN_DATA)
begin
   case mx2_sel is
      when "00" => DATA_WDATA <= IN_DATA; --OUTPUT
      when "01" => DATA_WDATA <= DATA_RDATA+1; -- +1
      when "11" => DATA_WDATA <= DATA_RDATA-1; -- -1
	  when others => DATA_WDATA <= DATA_RDATA;
   end case;
end process;  

--dekodovani instrukce
instr_decoding: process (DATA_RDATA)
   begin
     case (DATA_RDATA) is
		 when X"00" => instruction_decoded <= halt;
		 when X"3E" => instruction_decoded <= inc;
		 when X"3C" => instruction_decoded <= dec;
		 when X"2B" => instruction_decoded <= plus;
		 when X"2D" => instruction_decoded <= minus;
		 when X"5B" => instruction_decoded <= while_start;
		 when X"5D" => instruction_decoded <= while_end;
		 when X"2E" => instruction_decoded <= print;
		 when X"2C" => instruction_decoded <= load;
		 when others => instruction_decoded <= nop;
     end case;
   end process;  
   
  
--FSM present state
fsm_pstate: process(RESET, CLK)
   begin
      if (RESET='1') then
         pstate <= sidle;
      elsif (CLK'event) and (CLK='1') then
         if (EN = '1') then
            pstate <= nstate;
         end if;
      end if;
   end process;
   
fsm_nstate: process(pstate, OUT_BUSY, IN_VLD, IN_DATA, instruction_decoded, DATA_RDATA, cnt_reg_addr)
   begin   
   
	nstate <= sidle;
    IN_REQ <= '0';
    OUT_WE <= '0';
	
    pc_reg_inc <= '0';
    pc_reg_dec <= '0';
	
    cnt_reg_inc <= '0';
    cnt_reg_dec <= '0';
	
    ptr_reg_inc <= '0';
    ptr_reg_dec <= '0';
	
	mx1_sel <= '0';
	mx2_sel <= "11";
	
    DATA_EN <= '0';
    DATA_RDWR <= '0';

      case pstate is
         when sidle =>
            nstate <= sfetch;

         when sfetch =>
            nstate <= sdecode;
			DATA_EN <= '1';
            mx1_sel <= '0';
			

         when sdecode =>
            case instruction_decoded is
             when halt =>
                nstate <= shalt;
             when inc =>
                nstate <= sinc;
             when dec =>
                nstate <= sdec;
             when plus =>
                nstate <= splus0;
             when minus =>
                nstate <= sminus0;
			 when load =>
                nstate <= sload0;
             when print =>
                nstate <= sprint0;
             when while_start =>
                nstate <= swstart0;
             when while_end =>
                nstate <= swend0;
             when nop => 
                nstate <= snop;
            end case;

         when shalt =>
            nstate <= shalt;

         when snop =>
             nstate <= sfetch;
             pc_reg_inc <= '1';

         when splus0 =>
            nstate <= splus1;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			mx1_sel <= '1';
			
		when splus1 =>
            nstate <= sfetch;
			
			DATA_EN <= '1';
			DATA_RDWR <= '1'; --zapis !!!
			
			mx1_sel <= '1';
			mx2_sel <= "01";
			
            pc_reg_inc <= '1'; 
			
         when sminus0 =>
            nstate <= sminus1;
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			
			mx1_sel <= '1';
		 when sminus1 =>
            nstate <= sfetch;
			
			DATA_EN <= '1';
			DATA_RDWR <= '1';
			
			mx1_sel <= '1';
			mx2_sel <= "11";
            pc_reg_inc <= '1';		

         when sinc =>
            nstate <= sfetch;
            pc_reg_inc <= '1';
            ptr_reg_inc <= '1';

         when sdec =>
            nstate <= sfetch;
            pc_reg_inc <= '1';
            ptr_reg_dec <= '1';

         when sprint0 =>
            nstate <= sprint1;
            DATA_EN <= '1';
            DATA_RDWR <= '0';
			mx1_sel <= '1';
 
         when sprint1 =>
            nstate <= sprint1;
            if (OUT_BUSY='0') then
              nstate <= sfetch;
              OUT_WE <= '1';
			  OUT_DATA <= DATA_RDATA;
              pc_reg_inc <= '1';
            end if;      

         when sload0 =>
            nstate <= sload1;
            IN_REQ <= '1';
			mx1_sel <= '1';
			DATA_EN <= '1'; --mozna blbe
			DATA_RDWR <= '0'; --mozna blbe

         when sload1 =>
            IN_REQ <= '1';
            nstate <= sload1;
            if (IN_VLD='1') then
              nstate <= sfetch;
			  mx1_sel <= '1';
			  mx2_sel <= "00";
			  
              DATA_EN <= '1';
              DATA_RDWR <= '1';
			  
              pc_reg_inc <= '1';
            end if;

         when swstart0 =>
            nstate <= swstart1;		
			
			mx1_sel <= '1';
            DATA_EN <= '1';
            DATA_RDWR <= '0';			
			
			pc_reg_inc <= '1';
	  
         when swstart1 =>
            if ( DATA_RDATA = X"00" ) then
              nstate <= swstart2;		
            else 
              nstate <= sfetch;
            end if;
			
		when swstart2 =>
			nstate <= swstart3;
			DATA_EN <= '1';
            DATA_RDWR <= '0';

		when swstart3 =>
			nstate <= swstart2;		
			
			if( instruction_decoded = while_end) then
				if ( cnt_reg_addr = 0 ) then
					nstate <= sfetch;
				else
					cnt_reg_dec <= '1';
				end if;					
			elsif ( instruction_decoded = while_start ) then
				cnt_reg_inc <= '1';
			end if;
			
            pc_reg_inc <= '1';
			
		when swend0 =>
			nstate <= swend1;
			
			mx1_sel <= '1';
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			
		when swend1 =>
			nstate <= sfetch;			
			if( DATA_RDATA = X"00" ) then		
				pc_reg_inc <= '1';
			else
				nstate <= swend2;
				pc_reg_dec <= '1';
			end if;	
		
		when swend2 =>
			nstate <= swend3;
			mx1_sel <= '0';
			DATA_EN <= '1';
			DATA_RDWR <= '0';
		
			
		when swend3 =>
			nstate <= swend2;
			if ( instruction_decoded = while_start ) then
				if (cnt_reg_addr = 0 ) then
					pc_reg_inc <= '1';
					nstate <= sfetch;
				else
					cnt_reg_dec <= '1';	
					pc_reg_dec <= '1';
				end if;
				
			elsif ( instruction_decoded = while_end) then
				cnt_reg_inc <= '1';
				pc_reg_dec <= '1';				
			else
				pc_reg_dec <= '1';
			end if;
        when others =>
      end case;
   end process; 


end behavioral;
 
