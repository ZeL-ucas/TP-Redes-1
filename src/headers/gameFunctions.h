
    //inclue o connection functions para ter acesso a struct
    #include "connectionFunctions.h"


    /**
     * @brief Monta a mensagem necessaria baseada no type da gameMessage
     *
     * Recebe a gameMessage e usa os dados internos dela com algumas manipulaçoes para criar o message apropriado
     *
     * @param currentMessage mensagem a ser alterada
     * @return void
     */
    void EnumToString( GameMessage *currentMessage);

    /**
     * @brief Gera o numero aleatorio do servidor e chama a comparaçao, armazena no mainmessage a açao do server
     *
     * Recebe a gameMessage e usa os dados internos dela com algumas manipulaçoes para criar o message apropriado
     *
     * @param mainMessage mensagem principal de troca 
     * @return 0 para empate, -1 pra cliente, 1 pra servidor 
     */
    int PlayGame (GameMessage *mainMessage);
    /**
     * @brief Compara os ataques dos entre server e client 
     *
     * 
     *
     * @param server ataque do server
     * @param player ataque do servidor
     * @return 0 para cliente, 1 pra servidor 
     */
    int CheckWinner(int server,int player);
    /**
     * @brief Inicializa a struct com tudo zero pra evitar resultados inesperados
     *
     * 
     *
     * @param mainMessage struct a ser inicializada
     * @return void
     */
    void InitializateMainMessage(GameMessage *mainMessage);
        /**
     * @brief Faz o processo inicial de iniciar o jogo 
     *
     * 
     *
     * @param clientSocket socket do cliente pra enviar a mensagem de escolhas
     * @param mainMessage mensagem a ser editada
     * @return void
     */
    void StartGame(int clientSocket,GameMessage *mainMessage);