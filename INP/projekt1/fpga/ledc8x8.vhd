-- 1. Projekt do INP, vypracoval student: Martin Kačmarčík, xkacma03

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

entity ledc8x8 is
port
(
	RESET: in std_logic;
	SMCLK: in std_logic;
	ROW: out std_logic_vector(0 to 7);
	LED: out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture ledky of ledc8x8 is
	signal ce: std_logic; --clock enable
	signal rowCounter: std_logic_vector(7 downto 0); --citac radku
	signal control: std_logic_vector(7 downto 0); -- kontrolni promena pro zmeny radku
	signal ledRadek: std_logic_vector(7 downto 0); --promena pro rozsvicovani LED diod
	signal rowActive: std_logic; --promena pro aktivace/deaktivace radku

	--zmena aktivace radku:
	
	begin
		process (SMCLK, RESET, control, ce)
		begin
			if RESET = '1' then
				control <= (others => '0');
			elsif SMCLK'event and SMCLK = '1' then
				control <= control + 1;
			end if;
			
			if (control(7 downto 0) = "11111111") then -- SMCLK/256
				ce <= '1';
			else
				ce <= '0';
			end if;
			rowActive <= control(7);
		end process;
		
		--zmena aktivniho radku, zmenu provadim pomoci konkatenace 1. bitu, který tedy rotuje. 
		
		process(RESET, SMCLK, ce)
		begin
			if (RESET = '1') then
				rowCounter <= "00000001";
			elsif (SMCLK'event and SMCLK = '1' and ce = '1') then
				rowCounter <= rowCounter(0) & rowCounter(7 downto 1);
			end if;
		end process;
		
		--Rozsvicovani diod
		
		process(rowCounter)
		begin
			case rowCounter is
				when "00000001" => ledRadek <= "11110110";
				when "00000010" => ledRadek <= "11110101";
				when "00000100" => ledRadek <= "11110011";
				when "00001000" => ledRadek <= "01110101";
				when "00010000" => ledRadek <= "01110110";
				when "00100000" => ledRadek <= "01010111";
				when "01000000" => ledRadek <= "00100111";
				when "10000000" => ledRadek <= "01110111";
				when others =>     ledRadek <= "11111111";
			end case;
		end process;	
		
		--Prirazeni hodnot OUT signalum ROW a LED
		
		process(rowActive)
		begin
			ROW <= rowCounter;
			if(rowActive = '1') then
				LED <= ledRadek;
			else
				LED <= "11111111"; 
			end if;
		end process;

end ledky;	