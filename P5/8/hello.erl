-module(hello).
-export([start/0, salutation/2]).

start() ->
    spawn(fun() -> safe_init() end).

safe_init() ->
    process_flag(trap_exit, true),
    Pid = spawn_link(fun()-> init() end),
    loop_safe(Pid).

loop_safe(Pid) ->
    receive 
        {'EXIT', Pid, _} ->
            loop_safe(spawn_link(fun() -> init() end))
    end.

init() ->
    salutation(9001, 1500).

salutation (H, P) ->
    if  H < P ->
            receive after H -> exit(falla_desconocida) end;
        true -> 
            receive
            after
                P -> 
                    io:fwrite("Hola~p~n", [self()]),
                    hello:salutation(H - P, P)
            end
    end.



