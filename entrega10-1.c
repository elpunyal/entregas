void* AtenderCliente(void* socket)
{
    int sock_conn = *(int*)socket;
    char peticion[MAX], respuesta[MAX], notificacion[MAX];
    int nump, horas[24], sockets[MAX];

    // Enviar lista inicial de partidas abiertas
    strcpy(notificacion, "0/");
    nump = dameHoras(tpa, horas);
    for (int i = 0; i < nump; i++)
        sprintf(notificacion, "%s/%d", notificacion, horas[i]);
    write(sock_conn, notificacion, strlen(notificacion));

    int fin = 0, cambios = 0;
    while (fin == 0)
    {
        int ret = read(sock_conn, peticion, sizeof(peticion));
        peticion[ret] = '\0';
        char* p = strtok(peticion, "/");
        int codigo = atoi(p);

        switch (codigo)
        {
            case 0: // Desconexión
                fin = 1;
                pthread_mutex_lock(&mutex);
                cambios = eliminaJugador(&tpa, sock_conn);
                pthread_mutex_unlock(&mutex);
                break;

            case 1: // Abrir partida
                p = strtok(NULL, "/");
                int hora = atoi(p);
                pthread_mutex_lock(&mutex);
                int ok = crearPartida(&tpa, hora, sock_conn);
                pthread_mutex_unlock(&mutex);

                if (ok)
                {
                    strcpy(respuesta, "1/OK");
                    cambios = 1;
                }
                else
                    strcpy(respuesta, "1/YA EXISTE");
                write(sock_conn, respuesta, strlen(respuesta));
                break;

            case 2: // Apuntarse a partida
                p = strtok(NULL, "/");
                int hora = atoi(p);
                pthread_mutex_lock(&mutex);
                int cuantos = añadirJugador(&tpa, hora, sock_conn);
                pthread_mutex_unlock(&mutex);

                if (cuantos != 0)
                    strcpy(respuesta, "2/OK");
                else
                    strcpy(respuesta, "2/NO EXISTE");
                write(sock_conn, respuesta, strlen(respuesta));

                // Si la partida se completa
                if (cuantos == 4)
                {
                    int n, jugadores[4];
                    n = dameJugadores(tpa, hora, jugadores);
                    sprintf(notificacion, "3/%d", hora);
                    for (int i = 0; i < n; i++)
                        write(jugadores[i], notificacion, strlen(notificacion));
                    pthread_mutex_lock(&mutex);
                    eliminaPartida(&tpa, hora);
                    pthread_mutex_unlock(&mutex);
                    cambios = 1;
                }
                break;
        }
        if (cambios)
        {
            strcpy(notificacion, "0/");
            nump = dameHoras(tpa, horas);
            for (int i = 0; i < nump; i++)
                sprintf(notificacion, "%s/%d", notificacion, horas[i]);
            int numconectados = dameConectados(conectados, sockets);
            for (int i = 0; i < numconectados; i++)
                write(sockets[i], notificacion, strlen(notificacion));
            cambios = 0;
        }
    }
    close(sock_conn);
}


public partial class Form1 : Form
{
    Socket server;
    Thread atender;

    public Form1()
    {
        InitializeComponent();
    }

    private void conectar_Click(object sender, EventArgs e)
    {
        IPAddress direc = IPAddress.Parse("XXXXX");
        IPEndPoint ipep = new IPEndPoint(direc, YYYY);

        server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        try
        {
            server.Connect(ipep);
            this.BackColor = Color.Green;
            atender = new Thread(AtenderServidor);
            atender.Start();
        }
        catch (SocketException ex)
        {
            MessageBox.Show("No he podido conectar con el servidor");
            return;
        }
    }

    private void crearPartida_Click(object sender, EventArgs e)
    {
        string mensaje = "1/" + hora.Text;
        byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
        server.Send(msg);
    }

    private void apuntarsePartida_Click(object sender, EventArgs e)
    {
        string mensaje = "2/" + hora.Text;
        byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
        server.Send(msg);
    }

    private void desconectar_Click(object sender, EventArgs e)
    {
        atender.Abort();
        string mensaje = "0/";
        byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
        server.Send(msg);
        server.Close();
    }

    private void PonHoras(string[] trozos)
    {
        partidasGridView.Rows.Clear();
        for (int i = 1; i < trozos.Length; i++)
            partidasGridView.Rows.Add(trozos[i]);
    }

    private void AtenderServidor()
    {
        while (true)
        {
            byte[] msg = new byte[80];
            server.Receive(msg);
            string mensaje = Encoding.ASCII.GetString(msg).Split('\0')[0];
            string[] trozos = mensaje.Split('/');

            int op = Convert.ToInt32(trozos[0]);
            switch (op)
            {
                case 1:
                    this.Invoke(new Action(() => {
                        this.respuestaLbl.Text = trozos[1];
                    }));
                    break;
                case 2:
                    this.Invoke(new Action(() => {
                        this.respuestaLbl.Text = trozos[1];
                    }));
                    break;
                case 3:
                    this.Invoke(new Action(() => {
                        this.respuestaLbl.Text = "Partida completada a las " + trozos[1];
                    }));
                    break;
                case 0:
                    this.Invoke(new Action(() => {
                        PonHoras(trozos);
                    }));
                    break;
            }
        }
    }
}
