// pages/main/main.js
const app = getApp()

Page({

  /**
   * 页面的初始数据
   */
  data: {
    winWidth: 0,
    winHeight: 0,
    // tab切换  
    currentTab: 0, 

    // 用户信息
    userInfo: {},

    // 页面标题
    pageNameUserStatus: "收支明细",
    pageNameGroupManagement: "群记账", 
    pageNNameAbout: "关于",

    info: "暂无",

    // API
    urlAPIBase:'http://127.0.0.1/api/',
  },

  bindChange: function (e) {
    var that = this;
    that.setData({ currentTab: e.detail.current });
  },
  // 点击tab切换 
  swichNav: function (e) {
    var that = this;
    if (this.data.currentTab === e.target.dataset.current) {
      return false;
    } else {
      that.setData({
        currentTab: e.target.dataset.current
      })
    }
  },  

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
    var that = this;
    // 获取系统信息 
    wx.getSystemInfo({
      success: function (res) {
        that.setData({
          winWidth: res.windowWidth,
          winHeight: res.windowHeight
        });
      }
    });

    that.setData({
      userInfo: app.globalData.userInfo,
    });

    that.setData({
      urlAPIUserBase: that.data.urlAPIBase + 'user/',
      urlAPIGroupBase: that.data.urlAPIBase + 'group/',
      urlAPIUserBase: that.data.urlAPIBase + 'user/',
    });

    that.setData({
      urlAPIQueryUserStatus: that.data.urlAPIUserBase + 'status/',
    });

    wx.request({
      url: that.data.urlAPIQueryUserStatus + that.data.userInfo.nickName,
      success: function (res) {
        console.log(res.header);
        console.log(res.statusCode);
        if (res.data != null) {
          var groups = res.data.groups.join(', ')
          var relations = [];
          for (var i = 0; i < res.data.relation.length; i++) {
            relations.push(res.data.relation[i].join(": "));
          }
          var relationString = relations.join(', ');
          var resString = `Name: ${res.data.name}\nGroups: ${groups}\nRelations: ${relationString}`;
          that.setData({
            info: resString,//JSON.stringify(res.data, null, '  '),
          })
        } else {
          that.setData({
            info: "Getting Null Response!"
          })
        }
      } 
    });

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {
  
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
  
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
  
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
  
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
  
  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
  
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
  
  }
})