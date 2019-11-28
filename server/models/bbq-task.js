module.exports = (sequelize, type) => {
  return sequelize.define('bbq-task', {
    id: {
      type: type.INTEGER,
      primaryKey: true,
      autoIncrement: true
    },
    throughput: {
      type: type.INTEGER,
      allowNull: false
    },
    completion_time: {
      type: type.STRING,
      allowNull: false
    },
    n_threads: {
      type: type.INTEGER,
      allowNull: false
    },
    bbq_id: {
      type: type.INTEGER,
      allowNull: false
    }
  }, {
    underscored: true
  })
}