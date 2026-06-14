-module(intro).
-export([init/0]).

match_test () ->
    {A,B} = {5,4},
    %{C,C} = {5,4},
    {B,A} = {4,5},
    {D,D} = {5,5}.

string_test () -> [
    helloworld == 'helloworld',
    "helloworld" < 'helloworld',
    helloworld == "helloworld",
    [$h,$e,$l,$l,$o,$w,$o,$r,$l,$d] == "helloworld",
    [104,101,108,108,111,119,111,114,108,100] < {104,101,108,108,111,119,111,114,108,100},
    [104,101,108,108,111,119,111,114,108,100] > 1,
    [104,101,108,108,111,119,111,114,108,100] == "helloworld"].


tuple_test (P1, P2) ->
    io:fwrite("El nombre de P1 es ~p y el apellido de P2 es ~p~n", [nombre(P1), apellido(P2)]).

apellido ({persona, _, {apellido, A}}) -> A.
nombre ({persona, {nombre, A}, _}) -> A.

filtrar_por_apellido([], _) -> [];
filtrar_por_apellido([{persona, {nombre, N}, {apellido, A}}|L], Apellido) -> 
    Res = if 
        A == Apellido -> [N|(filtrar_por_apellido(L,Apellido))];
        true ->          filtrar_por_apellido(L,Apellido)
    end.

filtrar_por_apellido2(L, Apellido) -> [nombre(X) || X <- L, apellido(X) == Apellido].

init () ->
    P1 = {persona, {nombre, "Juan"}, {apellido, "Gomez"}},
    P2 = {persona, {nombre, "Carlos"}, {apellido, "Garcia"}},
    P3 = {persona, {nombre, "Javier"}, {apellido, "Garcia"}},
    P4 = {persona, {nombre, "Rolando"}, {apellido, "Garcia"}},
    match_test(),
    tuple_test(P1, P2),
    string_test(),
    Garcias = filtrar_por_apellido([P4, P3, P2, P1], "Garcia"),
    Garcias2 = filtrar_por_apellido2([P4, P3, P2, P1], "Garcia").
