# Fuel required to launch a given module is based on its mass.
# Specifically, to find the fuel required for a module, 
# take its mass, divide by three, round down, and subtract 2.

defmodule Aoc do

defp accumulate_fuel_from_file(fd, acc) do
  case IO.read(fd, :line)  do
    :eof -> 
      File.close fd
      acc
    {:error, reason} -> 
      IO.puts :stderr, reason
      :error
    line -> 
      # IO.puts String.to_integer String.trim line
      # same  as:
      # line |> String.trim |> String.to_integer |> IO.puts
      
      mass = line |> String.trim |> String.to_integer
      fuel = div(mass, 3) - 2
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
