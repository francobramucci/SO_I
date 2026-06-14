-module(servidor_turnos).
-export([server/0]).

% Para evitar errores de mensajes separados o pegados usamos {packet, line} que
% lee hasta encontrar un salto de línea (\n)

server() ->
    {ok, ListenSocket} = gen_tcp:listen(8000, [{reuseaddr, true}, {packet, line}]),
    Cont = init_cont(0),
    wait_connect(ListenSocket, Cont).

wait_connect(ListenSocket, Cont) ->
    {ok, Socket} = gen_tcp:accept(ListenSocket),
    spawn(fun () -> wait_connect (ListenSocket, Cont) end),
    get_request(Socket, Cont).

get_request(Socket, Cont) ->
    io:fwrite("Esperando mensajes de ~p~n", [Socket]),
    receive
        {tcp, Socket, "NUEVO\n"} ->
            N = inc_cont(Cont),
            gen_tcp:send(Socket, integer_to_list(N)),
            get_request(Socket, Cont);

        {tcp, Socket, "CHAU\n"} ->
            io:fwrite("Se ha cerrado la conección del socket: ~p~n", [Socket]);

        {tcp_closed, Socket} -> 
            io:fwrite("Se ha cerrado la conección del socket: ~p~n", [Socket]);

        _ -> get_request(Socket, Cont)
    end.

init_cont(N) ->
    spawn(fun() -> loop_cont(N) end).

inc_cont(Pid) ->
    Pid ! {inc, self()},
    receive
        {incOk, N} -> N
    end.

loop_cont(N) ->
    receive 
        {inc, Pid} -> Pid ! {incOk, N}, 
                       loop_cont(N+1)
    end.










