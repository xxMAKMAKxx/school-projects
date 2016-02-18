-- fsm.vhd: Finite State Machine
-- Author(s): Martin Kačmarčík , xkacma03
-- Kody: xkacma03 : kod1 = 973653464 	 kod2 = 9622755774
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (PODM0, PODM1, PODM2, PODM3, PODM4, PODM5, PODM6, PODM7, PODM8, PODM9, PODM10, PODM11, PODM12, PODM13, PODM14, PODM15, PODM16, WRONG, OK, WRT_OK, WRT_WRONG, DISPLAY);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= PODM0;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM0 =>
      next_state <= PODM0;
      if (KEY(9) = '1') then
         next_state <= PODM1;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
  
   when PODM1 =>
      next_state <= PODM1;
      if (KEY(6) = '1') then
         next_state <= PODM2;
	  elsif (KEY(7) = '1') then
		 next_state <= PODM10;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM2 =>
      next_state <= PODM2;
      if (KEY(2) = '1') then
         next_state <= PODM3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM3 =>
      next_state <= PODM3;
      if (KEY(2) = '1') then
         next_state <= PODM4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM4 =>
      next_state <= PODM4;
      if (KEY(7) = '1') then
         next_state <= PODM5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM5 =>
      next_state <= PODM5;
      if (KEY(5) = '1') then
         next_state <= PODM6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM6 =>
      next_state <= PODM6;
      if (KEY(5) = '1') then
         next_state <= PODM7;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM7 =>
      next_state <= PODM7;
      if (KEY(7) = '1') then
         next_state <= PODM8;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM8 =>
      next_state <= PODM8;
      if (KEY(7) = '1') then
         next_state <= PODM9;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM9 =>
      next_state <= PODM9;
      if (KEY(4) = '1') then
         next_state <= OK;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM10 =>
      next_state <= PODM10;
      if (KEY(3) = '1') then
         next_state <= PODM11;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM11 =>
      next_state <= PODM11;
      if (KEY(6) = '1') then
         next_state <= PODM12;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -

  when PODM12 =>
      next_state <= PODM12;
      if (KEY(5) = '1') then
         next_state <= PODM13;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM13 =>
      next_state <= PODM13;
      if (KEY(3) = '1') then
         next_state <= PODM14;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM14 =>
      next_state <= PODM14;
      if (KEY(4) = '1') then
         next_state <= PODM15;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM15 =>
      next_state <= PODM15;
      if (KEY(6) = '1') then
         next_state <= PODM16;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM16 =>
      next_state <= PODM16;
      if (KEY(4) = '1') then
         next_state <= OK;
      elsif (KEY(14 downto 0) /= "000000000000000") then
         next_state <= WRONG; 
      elsif(KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when OK =>
      next_state <= OK;
      if (KEY(15) /= '1') then
         next_state <= WRONG;
      elsif (KEY(15) = '1') then
         next_state <= WRT_OK;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRONG =>
      next_state <= WRONG; 
      if (KEY(15) /= '1') then
         next_state <= WRONG; 
      elsif (KEY(15) = '1') then
         next_state <= WRT_WRONG; 
      end if;   
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRT_OK =>
      next_state <= WRT_OK;
      if (CNT_OF = '1') then
         next_state <= DISPLAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when WRT_WRONG =>
      next_state <= WRT_WRONG;
      if (CNT_OF = '1') then
         next_state <= DISPLAY;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -   
    when DISPLAY =>
      next_state <= DISPLAY;
      if (KEY(15) = '1') then
         next_state <= PODM0; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= PODM0;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PODM0 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM1 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM2 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM3 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM4 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM5 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM6 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM7 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM8 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM9 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM10 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM11 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM12 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM13 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM14 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM15 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
      when PODM16 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   
   when WRT_WRONG =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
      
   when WRT_OK =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
	  FSM_MX_MEM	 <= '1';
   -- - - - - - - - - - - - - - - - - - - - - - -
         
   when OK =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
         
   when WRONG =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   
   when DISPLAY =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;

