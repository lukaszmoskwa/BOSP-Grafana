# BOSP-Grafana



### Setup

The setup of the whole application is divided into several phases:

+ Installation of Grafana
+ Cloning of BOSP-Grafana from Github.
+ Configuration of the MySQL database
+ NodeJS Server Configuration
+ Application launch

#### Installation of Grafana

To install Grafana's server from an Ubuntu-based system, simply run the following command:

```bash
sudo apt install grafana
```



For installation on other operating systems, please refer directly to the official page (https://grafana.com/docs/grafana/latest/installation/)

#### Cloning of BOSP-Grafana from Github

To obtain all the software related to the application, run the following command

```bash
git clone https://github.com/Lykos94/BOSP-Grafana
```



#### MySQL database configuration

In order to use the application correctly, you must configure a MySQL database on the machine on which you intend to run the server. First you need to install mysql-server

```bash
sudo apt install mysql-server
```



Then create a new user with password, and add a new database

```sql
mysql> CREATE DATABASE bosp-db
```



This will be necessary when configuring the server to save the data.

#### NodeJS Server Configuration


To configure the NodeJS server you need to enter the relative  application folder. To install all the dependencies necessary to properly work, run the command:

```bash
npm install
```

Now edit the file sequelize.js with the information of the newly created database 

```js
const sequelize = new Sequelize('bosp-db', 
    '<db-username>', '<db-password>', {
  host: 'localhost',
  dialect: 'mysql',
  logging: false,
});
```

#### Plugin setup

To install the plugin correctly you need to move the tree-table-plugin folder to the default location. By default, this corresponds to /var/lib/grafana/plugins.

```bash
cp tree-table-plugin/ /var/lib/grafana/plugins/
```

Then restart the grafana service (if already started)

```bash
sudo service grafana-server restart
```

Once this is done, the plugin is available in the dashboard

#### Application launch

Once the configuration has been completed, the applications must be started in the correct order to properly work.

```bash
sudo service grafana-server start
```

This will allow you to have a default web server on port 3000 with default user and password (admin:admin). Then access the BOSP shell and start the daemon through the command 

```bash
bosp-shell> bbque-startd
```



Run the NodeJS server waiting to receive messages from the client. From the root folder of the server, run 

```bash
node index.js
```

Then start the data client with the necessary parameters

```bash
./grafana_client 127.0.0.1 30200 30100 3 0 1000 3
```

The first 7 parameters that can be seen are those of the original BOSP data client and are documented at the following address: https://bosp.deib.polimi.it/doku.php?id=docs:api:dci#compilation.
There is also an additional eighth parameter that has been added to customize the address of the NodeJS server to which the data collected by Barbeque is sent: when this is not entered, by default it corresponds to http://locahost:3131/ (Note that in addition to the host is both the protocol and the port are specified). In this case it is assumed that the Nodes server that saves the database information is on the local machine running on port 3131.

The port in question can also be modified directly on the index.js file inside the server folder.

Translated with www.DeepL.com/Translator (free version)

```bash
# Example
./grafana_client 127.0.0.1 30200 30100 3 0 1000 3 \
    http://myserver.com:8080/
```


This will start the data client which receives from Barbeque regularly the data about the resource usage and the applications. At the same rate , which with these settings is every 1000 ms, the data client will send those information to the server which will receive and save them in the database.

It is now possible to login to the Grafana application. Connect locally to http://localhost:3000 (the default port is 3000 but you can change it). The first time it is started, you will be asked for information on how to connect Grafana to the database. After you have entered this information you can proceed with the creation of a dashboard.

If you want to use a previously created dashboard, simply import the JSON configuration file into the new dashboard and save. For more information, follow the detailed guide on the official page.
(https://grafana.com/docs/grafana/latest/reference/export_import/)