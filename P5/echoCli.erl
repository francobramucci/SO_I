-module(echoCli).
-export([cliente/0]).
-define(PUERTO, 1234).
-define(DIR, "localhost").

cliente() ->
    case gen_tcp:connect(?DIR, ?PUERTO, [binary, {active, false}]) of
        {ok, Socket} -> 
            gen_tcp:send(Socket, "hola"),
            case gen_tcp:recv(Socket, 0) of
                {ok, Packet} -> io:fwrite("Cliente - recibiendo ~p ~n", [Packet]),
                                gen_tcp:close(Socket);

                {error, closed} -> io:fwrite("Se cerró el Socket ~n", [])
            end;

        {error, Reason} -> 
            io:fwrite("Falló con la razón Socket~p ~n", [Reason])
    end.
