using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using NodeMCUAPI.Model;
using MySql.Data.MySqlClient;
using System.Data;
using NodeMCUAPI;


namespace NodeMCUAPI.Controllers
{
    [Route("[controller]")]
    public class TemperatureController : Controller
    {
        private string connectionString = "Server=127.0.0.1;Port=3306;Database=nodedb;Uid=root;password=####";

        // GET Function
        [HttpGet]
        public IEnumerable<Temperature> Get()
        {
            IList<Temperature> ListaTemperaturas = new List<Temperature>();

            MySqlConnection conexion = new MySqlConnection(connectionString);
            conexion.Open();

            MySqlCommand cmd = new MySqlCommand();
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "sp_get_temp";
            cmd.Connection = conexion;

            Temperature temperature1 = new Temperature();

            using (var reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    temperature1 = new Temperature();
                    temperature1.logId = Convert.ToInt32(reader["log_id"]);
                    temperature1.LogDate = Convert.ToDateTime(reader["log_date"]);
                    temperature1.deviceId = Convert.ToInt32(reader["device_id"]);
                    temperature1.temp = (float)Convert.ToDecimal(reader["temp"]);
                    ListaTemperaturas.Add(temperature1);
                }
            }
            conexion.Dispose();

            return ListaTemperaturas;
        }


        // POST Fucntion
        [HttpPost]
        public void Post([FromBody] Temperature temperatureItem)
        {
            Console.WriteLine("Entro al post");
            MySqlConnection conexion = new MySqlConnection(connectionString);
            conexion.Open();

            MySqlCommand cmd = new MySqlCommand();
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "sp_add_temp";
            cmd.Parameters.AddWithValue("@t_log_id", temperatureItem.logId);
            cmd.Parameters.AddWithValue("@t_log_date", DateTime.Now);
            cmd.Parameters.AddWithValue("@t_device_id", temperatureItem.deviceId);
            cmd.Parameters.AddWithValue("@t_temp", temperatureItem.temp);
            cmd.Connection = conexion;

            MySqlDataReader myReader = cmd.ExecuteReader();

            conexion.Close();
        }
    }
}