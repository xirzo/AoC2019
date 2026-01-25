defmodule Aoc do

defp interpret_inner(nums, i) do
  # check if out of bounds

  case elem(nums, i) do
    1 -> 
      first_operand = elem(nums, (elem nums, i + 1)) 
      second_operand = elem(nums, (elem nums, i + 2))
      index = (elem nums, i + 3)
      #  IO.puts "Add #{first_operand} and #{second_operand} and put to index: #{index}"
      nums = put_elem nums, index, first_operand + second_operand
      interpret_inner nums, i + 4
    2 -> 
      first_operand = elem(nums, (elem nums, i + 1)) 
      second_operand = elem(nums, (elem nums, i + 2))
      index = (elem nums, i + 3)
      # IO.puts "Multiply #{first_operand} and #{second_operand} and put to index: #{index}"
      nums = put_elem nums, index, first_operand * second_operand
      interpret_inner nums, i + 4
    99 -> 
      # IO.puts "Halt"
      elem(nums, 0)
  end
end

defp interpret_outter(nums, x, y) do
  nums = put_elem nums, 1, x
  nums = put_elem nums, 2, y
  interpret_inner nums, 0
end

defp interpret(nums) do
  for x <- 1..99, y <- 1..99 do
    output = interpret_outter nums, x, y
    if output == 19690720 do
      IO.puts 100 * x + y
    end
  end
end

defp process_line(line) do
  String.split(line, ",")
  |> Enum.map(&String.trim/1)
  |> Enum.map(&String.to_integer/1)
  |> Enum.to_list # :)
  |> List.to_tuple
  |> interpret
end

defp process_file(fd) do
  case IO.read fd, :line do
    :eof ->
      :ok
    line -> 
      process_line line
  end
end

def interpret_file(path) do
  case File.open path do
    {:ok, fd} ->
      process_file fd
    {:error, reason} ->
      IO.puts(:stderr, reason)
  end
end

end

Aoc.interpret_file "./input2.txt"
