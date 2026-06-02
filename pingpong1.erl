-module(pingpong1).
-export([ping/0, pong/0, play/0, ping_aux/1, pong_aux/0]).

% Ping
ping() -> 
    receive
        {fin, Pid} -> io:fwrite("Ping Terminó: ~p ~n", [Pid]);
                      
        {0, Pid}   -> io:fwrite("Ping recv: ~p ~p ~n", [0, Pid]), 
                      Pid ! {fin, self()};

        {4, _}   -> io:fwrite("Ping hizo throw() ~n", []),
                      throw(ping_muere);

        {3, _}   -> io:fwrite("Ping hizo throw() ~n", []),
                      throw(ping_muere);

        {N, Pid}   -> io:fwrite("Ping recv: ~p ~p ~n", [N, Pid]), 
                      Pid ! {N-1, self()}, ping()
    end.


ping_aux(PidPong) ->
    link(PidPong),
    ping().

% Pong
pong() -> 
    receive
        {fin, Pid} -> io:fwrite("Pong Terminó: ~p ~n", [Pid]);

        {0, Pid} -> io:fwrite("Pong recv: ~p ~p ~n", [0, Pid]), 
                    Pid ! {fin, self()}, okPong;

        {N, Pid} -> io:fwrite("Pong recv: ~p ~p ~n", [N, Pid]), 
                    Pid ! {N-1, self()}, pong()
        %{'EXIT', From, Reason} -> io:format("pong exiting, got ~p ~n", [{'EXIT', From, Reason}])
    end.

pong_aux() ->
    %process_flag(trap_exit, true),
    pong().

play() -> 
    PidPong = spawn(pingpong, pong_aux, []),
    PidPing = spawn(pingpong, ping_aux, [PidPong]),
    PidPong ! {10, PidPing},
    okPlay.

