#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *resultado;
    MYSQL_ROW row;
    int err;
    char consulta[100];

    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Error al crear la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    conn = mysql_real_connect(conn, "localhost", "user", "pass", "cartas", 0, NULL, 0);
    if (conn == NULL) {
        printf("Error al inicializar la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    strcpy(consulta, "SELECT * FROM mesas");
    err = mysql_query(conn, consulta);
    if (err != 0) {
        printf("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    resultado = mysql_store_result(conn);
    row = mysql_fetch_row(resultado);

    if (row == NULL) {
        printf("No se han obtenido datos en la consulta\n");
    } else {
        while (row != NULL) {
            printf("ID: %s, Carta: %s, Cantidad: %s\n", row[0], row[1], row[2]);
            row = mysql_fetch_row(resultado);
        }
    }

    mysql_free_result(resultado);
    mysql_close(conn);

    return 0;
}