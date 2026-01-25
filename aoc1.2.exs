defmodule Aoc do

defp calculate_fuel(mass) do
  fuel = div(mass, 3) - 2
  if fuel <= 0 do
    0
  else
    fuel + calculate_fuel fuel
  end
end

defp accumulate_fuel_from_file(fd, acc) do
  case IO.read(fd, :line)  do
    :eof -> 
      File.close fd
      acc
    {:error, reason} -> 
      IO.puts :stderr, reason
      :error
    line -> 
      mass = line |> String.trim |> String.to_integer
      fuel = calculate_fuel mass
      accumulate_fuel_from_file fd, acc + fuel
  end
end

def process_file(filename) do
  case File.open filename, [:read, :utf8] do
    {:ok, fd} -> 
      case accumulate_fuel_from_file fd, 0 do
        fuel ->
          IO.puts fuel
      end
    {:error, reason} -> 
      IO.puts :stderr, reason
  end
end

end

Aoc.process_file "input1.txt"
