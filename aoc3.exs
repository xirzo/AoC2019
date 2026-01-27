defmodule Aoc do
    defp parse_line(line) do
	String.trim(line)
	|> String.split(",")
	|> IO.puts
	|> 
    end

    def process_input(file_path) do
	with {:ok, fd} <- File.open(file_path),
	    line <- IO.read(fd, :line),
	    do: parse_line(line)
    end
end

Aoc.process_input "./input3.txt"
