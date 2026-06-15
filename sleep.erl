-module(sleep).
-export([sleep/1]).

sleep(T) ->
    receive
    after
        T -> ok
    end.

empty_mailbox() ->
    receive
        _ -> empty_mailbox();
    after
        1000 -> ok
    end.

priority_process() -> 
    receive
        msg1 -> ok;
    after
        500 ->
            receive
                msg2 -> ok
            end
    end.






