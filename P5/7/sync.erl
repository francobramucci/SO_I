-module(sync).
-export([createLock/0, lock/1, unlock/1, destroyLock/1]).
-export([createSem/1, semP/1, semV/1, destroySem/1]).
-export([testLock/0, testSem/0]).

createLock () ->
    spawn(fun() -> lock_actor(0, [], null) end).

lock_actor (E, L, O) ->
    receive
        {lock, Pid} ->
            if  E == 0 -> 
                    Pid ! pass,
                    lock_actor(1, L, Pid);
                true ->
                    lock_actor(1, L ++ [Pid], O)
            end;

        {unlock, Pid} ->
            if  Pid == O ->
                    case L of
                        [] -> 
                            lock_actor(0, [], Pid);
                        [H | T] ->
                            H ! pass,
                            lock_actor(1, T, H)
                    end;
                true ->
                    lock_actor(1, L, Pid)
            end;

        destroy -> okLockActor
    end.

lock (M) ->
    M ! {lock, self()},
    receive
        pass -> ok
    end.

unlock (M) ->
    M ! {unlock, self()}.

destroyLock (M) ->
    M ! destroy.

createSem (N) ->
    spawn(fun() -> sem_loop(N, []) end).

sem_loop(N, L) ->
    receive
        {p, Pid} ->
            if  N > 0 ->
                    Pid ! pass,
                    sem_loop(N-1, L);
                true ->
                    sem_loop(0, L ++ [Pid])
            end;

        v ->
            case L of
                [] -> 
                    sem_loop(N+1, []);
                [H | T] ->
                    H ! pass,
                    sem_loop(N, T)
            end;
        
        destroy -> okSemActor
    end.

semP (S) ->
    S ! {p, self()},
    receive
        pass -> ok
    end.

semV (S) ->
    S ! v.

destroySem (S) ->
    S ! destroy.

f (L, W) ->
    lock(L),
    % regioncritica(),
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    unlock(L),
    W ! finished.

waiter (L, 0) -> destroyLock(L);
waiter (L, N) -> receive finished -> waiter(L, N-1) end.

waiter_sem (S, 0) -> destroySem(S);
waiter_sem (S, N) -> receive finished -> waiter_sem(S, N-1) end.

testLock () ->
    L = createLock(),
    W = spawn(fun () -> waiter(L, 3) end),
    spawn (fun () -> f(L, W) end),
    spawn (fun () -> f(L, W) end),
    spawn (fun () -> f(L, W) end),
    ok.

sem (S, W) ->
    semP(S),
    %regioncritica(), bueno, casi....
    io:format("uno ~p~n", [self()]),
    io:format("dos ~p~n", [self()]),
    io:format("tre ~p~n", [self()]),
    io:format("cua ~p~n", [self()]),
    io:format("cin ~p~n", [self()]),
    io:format("sei ~p~n", [self()]),
    semV(S),
    W ! finished.

testSem () ->
    S = createSem(2), % a lo sumo dos usando io al mismo tiempo
    W = spawn (fun () -> waiter_sem (S, 5) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    spawn (fun () -> sem (S, W) end),
    ok.
