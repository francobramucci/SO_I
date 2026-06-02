-module(pingpong).
-export([ping/0, pong/0, play/0]).

% Ping
ping() -> 
    receive
        {fin, Pid} -> io:fwrite("Ping Terminó: ~p ~n", [Pid]);
                      
        {0, Pid}   -> io:fwrite("Ping recv: ~p ~p ~n", [0, Pid]), 
                      Pid ! {fin, self()};

        {N, Pid}   -> io:fwrite("Ping recv: ~p ~p ~n", [N, Pid]), 
                      Pid ! {N-1, self()}, ping()
    end.


% Pong
pong() -> 
    receive
        {fin, Pid} -> io:fwrite("Pong Terminó: ~p ~n", [Pid]);

        {0, Pid} -> io:fwrite("Pong recv: ~p ~p ~n", [0, Pid]), 
                    Pid ! {fin, self()}, okPong;

        {N, Pid} -> io:fwrite("Pong recv: ~p ~p ~n", [N, Pid]), 
                    Pid ! {N-1, self()}, pong()
    end.


play() -> 
    PidPing = spawn(pingpong, ping, []),
    PidPong = spawn(pingpong, pong, []),
    PidPong ! {10, PidPing},
    okPlay.

