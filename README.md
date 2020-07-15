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

## <b>Gateway -> quadro:</b>

macDoQuadro!nomeSala!valor(0 ou 1)

"0" para desligar a sala e "1" para ligar a sala

## <b>Gateway -> Sala:</b>

sala = esp32

MAC_DA_SALA!MAC_DO_GATEWAY!comando!valor

## <b> APLICATIVO </b>

esp_ar = sala

Mensagem do app: comando!sala!valor

Envia dai: macDoGateway!comando!sala!valor

SE o comando for "rt" = {"2"-"3" para desligar e ligar ar} e {"17 até 27" para temperatura}

SE o comando for "t" = {"5" - "10" - "15" - "30"} 

## <b>Comandos Sala -> Gateway:</b>

MAC_DO_GATEWAY!info!MAC_REMETENTE!temperatura!humidade!corrente!potencia!kwh

SE o comando for "rt" = {"2"-"3" para desligar e ligar ar} e {"17 até 27" para temperatura}

SE o comando for "t" = {"5" - "10" - "15" - "30"} 


///////////////////////////////////////////////////////////////////

Comando de confirmação: macRemetente!confir!macDestinatario

MAC padrão do Quadro :(MAC_quadro = 71:75:61:64:72:6f)
MAC padrão do Gateway : (MAC_banco = 62:61:6e:63:6f:0d:0a)  // não está atualizado (15/07/2020)
