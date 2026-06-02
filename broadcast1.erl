-module(broadcast1).

%% Librería
% Funciones control
-export([start/0,stop/0]).
% Funciones interacción
-export([broadcast/1,registrar/0]).

-export([loopBroadcast/1]).

-export([test/0,client/0]).


start() -> BroadcastPid = spawn(?MODULE, loopBroadcast, [[]]),
           register(servidor_bc, BroadcastPid),
           ok.

stop() -> servidor_bc ! stp.

broadcast(Msg) -> servidor_bc ! {bc, Msg}.

registrar() -> servidor_bc ! {reg, self()}.

loopBroadcast(RegisteredPids) ->
    receive
        {reg, Pid} -> io:fwrite("El cliente ~p se registró ~n", [Pid]),
                      loopBroadcast([Pid | RegisteredPids]);

        {bc, Msg}  -> lists:foreach(fun (Pid) -> Pid ! Msg end , RegisteredPids),
                      loopBroadcast(RegisteredPids);
        
        stp        -> io:fwrite("El servidor broadcast terminó ~n")
    end,
    okLoopBc.

client() ->
    registrar(),
    loopClient().

loopClient() ->
    receive 
        Msg -> io:fwrite("El cliente ~p recibió ~p ~n", [self(), Msg]),
               loopClient()
    end.

test() ->
    start(),
    spawn(?MODULE, client, []),
    spawn(?MODULE, client, []),
    timer:sleep(1000),
    broadcast(hola).






