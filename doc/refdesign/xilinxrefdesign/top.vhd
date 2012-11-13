
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
--library UNISIM;
--use UNISIM.VComponents.all;

entity top is Port ( 
  clk   : in   STD_LOGIC;
  led   : out  STD_LOGIC_VECTOR (15 downto 0));
end top;

architecture Behavioral of top is
  signal count_int : std_logic_vector(26 downto 0);
begin

  count_proc:process begin
    wait until rising_edge(clk);
	 count_int <= std_logic_vector(unsigned(count_int) + 1);
	 led <= count_int(count_int'left downto count_int'left-led'length+1);
  end process;


end Behavioral;

