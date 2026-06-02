-module(safe_div).
-export([safediv/2]).

safediv(N, M) ->
    case catch (N/M) of
        {'EXIT', {Reason, _}} -> io:format("División por cero capturada ~p ~n", [Reason]);
        Res -> Res
    end.
