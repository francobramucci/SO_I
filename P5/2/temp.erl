-module(temp).
-export([wait/1, cronometro/3]).


wait(T) ->
    receive 
    after 
        T -> ok
    end.

cronometro(Fun, Hasta, Periodo) ->
    if 
        Hasta < Periodo -> 
            receive 
            after
                Hasta -> ok
            end;

        true ->
            receive
            after
                Periodo -> Fun(), cronometro(Fun, (Hasta-Periodo), Periodo)
            end
    end.
