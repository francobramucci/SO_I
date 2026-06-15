-module(broadcast).
-export([iniciar_server/0, suscribir/0, enviar/1, desuscribir/0]).

iniciar_server() ->
    register(server, spawn(fun() -> loop_server([]) end)).

loop_server(L) ->
    receive
        {suscribir, Pid} -> 
            case lists:member(Pid, L) of
                true ->
                    loop_server(L);
                false ->
                    loop_server([Pid | L])
            end;

        {desuscribir, Pid} -> 
            loop_server(lists:delete(Pid, L));

        {enviar, Msg} -> 
            bcast(L, Msg),
            loop_server(L)
    end.

bcast([], _) -> ok;
bcast([H | T], Msg) ->
    H ! Msg,
    bcast(T, Msg).


suscribir() ->
    server ! {suscribir, self()}.

desuscribir() ->
    server ! {desuscribir, self()}.

enviar(Msg) ->
    server ! {enviar, Msg}.
