-module(sleep).
-export([sleep/1]).

sleep(T) ->
    receive
    after
        T -> ok
    end.


