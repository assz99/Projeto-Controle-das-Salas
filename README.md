# Projeto-Controle-das-Salas

40 segundos para enviar

Caminho para adicionar bibliotecas e arquivo do visual code para editar.

-> arquivo para edição: c_cpp_properties.json


-> C:\Users\<SEU_USUARIO>\.platformio



Comandos Banco -> quadro:

macDoQuadro!nomeSala!valor(0 ou 1)

"0" para desligar a sala e "1" para ligar a sala

Comandos Banco -> Sala:

macDaSala!comando!valor

SE o comando for "rt" = {"2"-"3" para desligar e ligar ar} e {"17 até 27" para temperatura}

SE o comando for "t" = {"5" - "10" - "15" - "30"} 

Comandos Sala -> Banco:

macPadraoDoReceiver!macLocal!temperatura!humidade!corrente!potencia!kwh

Mensagem do app: comando!sala!valor

Envia dai: macDoBanco!comando!sala!valor

Comando de confirmação: macRemetente!confir!macDestinatario

