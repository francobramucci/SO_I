-module(anillo).
-export([iniciar_anillo/1]).

int_to_atom(N) -> list_to_atom(integer_to_list(N)).

iniciar_anillo(N) -> iniciar(0, N).

iniciar(N, N) -> ok;
iniciar(I, N) ->
    Pid = spawn(fun() -> proceso(I, N) end),
    % Registro los procesos con un número.
    register(int_to_atom(I), Pid),
    iniciar(I+1, N).


proceso(I, N) ->
    Cur = int_to_atom(I),
    Sig = int_to_atom((I+1) rem N),
    receive
        {msg, 0} ->
            io:fwrite("Soy ~p y recibí ~p~n", [Cur, 0]),
            Sig ! exit,
            receive
                exit ->
                    io:fwrite("Soy ~p y recibí ~p~n", [Cur, exit])
            end;
        {msg, M} ->
            io:fwrite("Soy ~p y recibí ~p~n", [Cur, M]),
            Sig ! {msg, M-1},
            proceso(I, N);
        exit -> 
            io:fwrite("Soy ~p y recibí ~p~n", [Cur, exit]),
            Sig ! exit
    end.

