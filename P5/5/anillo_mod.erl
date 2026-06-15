-module(anillo_mod).
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
        msg -> 
            io:fwrite("Soy ~p, el primero en mandar el msg~n", [Cur]),
            Sig ! {msg, Cur},
            proceso(I, N);

        {msg, Cur} ->
            io:fwrite("Soy ~p, el msg dio la vuelta, lo descarto~n", [Cur]),
            Sig ! exit,
            receive
                exit ->
                    io:fwrite("Soy ~p y recibí ~p~n", [Cur, exit])
            end;

        {msg, Sender} ->
            io:fwrite("Soy ~p, reenvío el msg~n", [Cur]),
            Sig ! {msg, Sender},
            proceso(I, N);

        exit -> 
            io:fwrite("Soy ~p y recibí ~p~n", [Cur, exit]),
            Sig ! exit
    end.

