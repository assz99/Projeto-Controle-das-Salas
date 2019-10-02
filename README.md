# Projeto-Controle-das-Salas

40 segundos para enviar

Caminho para adicionar bibliotecas e arquivo do visual code para editar.

-> arquivo para edição: c_cpp_properties.json


-> C:\Users\<SEU_USUARIO>\.platformio

## <b>Comunicação Gateway-Node</b>

Gateway: 
RX:LoRa.disableInvertIQ();
TX:LoRa.enableInvertIQ(); 

Node:
RX:LoRa.enableInvertIQ(); 
TX:LoRa.disableInvertIQ(); 

## <b>Comandos Banco -> quadro:</b>

macDoQuadro!nomeSala!valor(0 ou 1)

"0" para desligar a sala e "1" para ligar a sala

## <b>Comandos Banco -> Sala:</b>

macDaSala!comando!valor

SE o comando for "rt" = {"2"-"3" para desligar e ligar ar} e {"17 até 27" para temperatura}

SE o comando for "t" = {"5" - "10" - "15" - "30"} 

## <b>Comandos Sala -> Banco:</b>

macPadraoDoReceiver!macLocal!temperatura!humidade!corrente!potencia!kwh

Mensagem do app: comando!sala!valor

Envia dai: macDoBanco!comando!sala!valor

Comando de confirmação: macRemetente!confir!macDestinatario

