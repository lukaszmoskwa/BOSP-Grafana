module.exports = (sequelize, type) => {
  return sequelize.define('bbq-app', {
    id: {
      type: type.INTEGER,
      primaryKey: true,
      autoIncrement: true
    },
    name: {
      type: type.STRING,
      allowNull: false
    },
    state: {
      type: type.STRING,
      allowNull: false
    }
  }, {
    underscored: true
  })
}