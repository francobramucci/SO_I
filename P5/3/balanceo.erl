bal(Servs) ->
    receive
        {req, Arg, Pid} ->
            Proc = lists:nth(rand:uniform(length(Servs)), Servs),
            Proc ! { req, Arg, self() },
    
            receive
                {ans, Reply} -> Pid ! Reply
            end
    end,
    bal(Servs).

% El problema es que en el receive de la línea 7 el proceso espera recibir
% cualquier mensaje. Si otro cliente envía una request entonces el proceso
% enviará esa request a otro cliente, en vez de enviar la respuesta del
% servidor. Para eso, se considera el átomo ans en los mensajes de respuesta,
% para que el proceso reconozca si el mensaje proviene de un cliente o del
% servidor.
