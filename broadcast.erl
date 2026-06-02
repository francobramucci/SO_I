-module(broadcast).

%% Librería
% Funciones control
-export([start/0,stop/1]).
% Funciones interacción
-export([broadcast/2,registrar/1]).

-export([loopBroadcast/1]).

-export([test/0,client/1]).


start() -> spawn(?MODULE, loopBroadcast, [[]]),
           ok.

stop(Broadcast) -> Broadcast ! stp.

broadcast(Broadcast, Msg) -> Broadcast ! {bc, Msg}.

registrar(Broadcast) -> Broadcast ! {reg, self()}.

loopBroadcast(RegisteredPids) ->
    receive
        {reg, Pid} -> io:fwrite("El cliente ~p se registró ~n", [Pid]),
                      loopBroadcast([Pid | RegisteredPids]);

        {bc, Msg}  -> lists:foreach(fun (Pid) -> Pid ! Msg end , RegisteredPids),
                      loopBroadcast(RegisteredPids);
        
        stp        -> io:fwrite("El servidor broadcast terminó ~n")
    end,
    okLoopBc.

client(Broadcast) ->
    registrar(Broadcast),
    loopClient().

loopClient() ->
    receive 
        Msg -> io:fwrite("El cliente ~p recibió ~p ~n", [self(), Msg]),
               loopClient()
    end.

test() ->
    Broadcast = start(),
    spawn(?MODULE, client, [Broadcast]),
    spawn(?MODULE, client, [Broadcast]),
    timer:sleep(1000),
    broadcast(Broadcast, hola),

    stop(Broadcast).






