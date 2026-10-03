    // === REPLACE WITH YOUR WEBHOOK URL ===
    const WEBHOOK_URL = "https://discord.com/api/webhooks/1555527280552452146/eVYYlrza15AJb0iRGeYw2qX3zi_1IMXHlqXKUS4KZe-aNBzP156R9mQNx9-V9_Ks8AwZ";
    // ======================================

    const form = document.getElementById('rokForm');
    const statusMsg = document.getElementById('statusMsg');
    const submitBtn = document.getElementById('submitBtn');
    const fileInput = document.getElementById('screenshot');
    const preview = document.getElementById('preview');

    fileInput.addEventListener('change', () => {
      const f = fileInput.files[0];
      if (!f) { preview.style.display='none'; return; }
      if (f.size > 8*1024*1024) {
        alert("File too large! Max 8MB.");
        fileInput.value = ''; preview.style.display='none'; return;
      }
      preview.src = URL.createObjectURL(f);
      preview.style.display = 'block';
    });

    form.addEventListener('submit', async e => {
      e.preventDefault();
      const file = fileInput.files[0];
      if (!file) { showStatus("❌ Attach your ROK profile screenshot.", true); return; }

      submitBtn.disabled = true;
      showStatus("Submitting...", false, true);

      const ign = document.getElementById('ign').value || 'Applicant';
      const discordUser = document.getElementById('discord').value;
      const userID = document.getElementById('discordid').value;

      const embed = {
        title: "📥 New ROK Application",
        color: 16766720,
        image: { url: "attachment://rok-proof.png" },
        fields: [
          { name: "Discord", value: discordUser, inline: true },
          { name: "User ID", value: userID, inline: true },
          { name: "IGN", value: ign, inline: true },
          { name: "Kingdom", value: document.getElementById('kingdom').value, inline: true },
          { name: "Might", value: Number(document.getElementById('might').value).toLocaleString(), inline: true },
          { name: "City Hall", value: `Lv. ${document.getElementById('chlevel').value}`, inline: true },
          { name: "Timezone", value: document.getElementById('timezone').value, inline: true },
          { name: "Voice Comms", value: document.getElementById('voice').value },
          { name: "Commanders", value: document.getElementById('commanders').value || "—" },
          { name: "Previous Alliance", value: document.getElementById('previous').value || "—" },
          { name: "Extra Info", value: document.getElementById('extra').value || "—" }
        ],
        timestamp: new Date().toISOString()
      };

      // Auto-ping R4 + R5 — replace IDs!
      const content = `<@&R5_ROLE_ID_HERE> <@&R4_ROLE_ID_HERE> — new application from ${discordUser}`;

      const fd = new FormData();
      fd.append('payload_json', JSON.stringify({
        thread_name: `App: ${ign}`,  // Auto-names the post!
        content: content,
        embeds: [embed],
        applied_tags: ["Pending"]   // Auto-tags it Pending
      }));
      fd.append('files[0]', file, 'rok-proof.png');

      try {
        const res = await fetch(WEBHOOK_URL, { method: "POST", body: fd });
        if (res.ok) {
          showStatus("✅ Submitted! Staff will review & add you to the conversation.", false);
          form.reset(); preview.style.display='none';
        } else { throw new Error("Error"); }
      } catch (err) {
        showStatus("❌ Failed. Check webhook URL.", true);
        console.error(err);
      }
      submitBtn.disabled = false;
    });

    function showStatus(msg, isError, loading) {
      statusMsg.textContent = msg;
      statusMsg.className = "status show " + (isError ? "error" : "success");
      if (loading) statusMsg.style.color = "#FCD34D";
      else statusMsg.style.color = "";
    }
        
