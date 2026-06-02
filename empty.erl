-module(empty).
-export([empty_mailbox/0]).

empty_mailbox() ->
    receive
        _ -> empty_mailbox()
    after
        0 -> ok
    end.
